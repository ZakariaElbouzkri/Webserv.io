#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cerrno>

#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/poll.h>

const int PORT = 8080;
const int MAX_CLIENTS = 10;

int createNonBlockingSocket() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set the socket to non-blocking mode
    fcntl(serverSocket, F_SETFL, O_NONBLOCK);

    return serverSocket;
}

void bindAndListen(int serverSocket) {
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("Bind failed");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, SOMAXCONN) == -1) {
        perror("Listen failed");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }
}

int acceptConnection(int serverSocket) {
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);
    int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);

    if (clientSocket == -1) {
        if (errno == EWOULDBLOCK || errno == EAGAIN) {
            // No incoming connections
            return -1;
        } else {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }
    }

    // Set the client socket to non-blocking mode
    fcntl(clientSocket, F_SETFL, O_NONBLOCK);

    std::cout << "New connection accepted: " << inet_ntoa(clientAddress.sin_addr) << ":" << ntohs(clientAddress.sin_port) << std::endl;

    return clientSocket;
}

void handleClient(int clientSocket) {
    // Read the HTML file
    FILE* file = fopen("index.html", "r");
    if (!file) {
        perror("Failed to open HTML file");
        close(clientSocket);
        return;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = new char[fileSize + 1];
    fread(buffer, 1, fileSize, file);
    fclose(file);

    buffer[fileSize] = '\0';

    // Send the HTTP response
    const char* httpResponse =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Connection: close\r\n"
        "Content-Length: %ld\r\n"
        "\r\n%s";

    char responseBuffer[4096];
    snprintf(responseBuffer, sizeof(responseBuffer), httpResponse, fileSize, buffer);

    ssize_t bytesSent = send(clientSocket, responseBuffer, strlen(responseBuffer), 0);

    if (bytesSent < 0) {
        perror("Send failed");
        close(clientSocket);
    }

    delete[] buffer;
    close(clientSocket);
}

int main() {
    int serverSocket = createNonBlockingSocket();
    std::vector<pollfd> pollFds;

    bindAndListen(serverSocket);

    // Add the server socket to the poll set
    pollfd serverPollFd;
    serverPollFd.fd = serverSocket;
    serverPollFd.events = POLLIN;
    pollFds.push_back(serverPollFd);

    while (true) {
        int activity = poll(pollFds.data(), pollFds.size(), -1);

        if (activity < 0) {
            perror("Poll error");
            exit(EXIT_FAILURE);
        }

        // If something happened on the server socket, then it's an incoming connection
        if (pollFds[0].revents & POLLIN) {
            int newSocket = acceptConnection(serverSocket);
            if (newSocket != -1) {
                // Add the new client socket to the poll set
                pollfd clientPollFd;
                clientPollFd.fd = newSocket;
                clientPollFd.events = POLLIN;
                pollFds.push_back(clientPollFd);
            }
        }

        // Handle data for connected clients
        for (size_t i = 1; i < pollFds.size();) {
            if (pollFds[i].revents & POLLIN) {
                handleClient(pollFds[i].fd);

                // Remove the client from the poll set
                pollFds.erase(pollFds.begin() + i);
            } else {
                ++i;
            }
        }
    }

    close(serverSocket);

    return 0;
}
