#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/poll.h>

const int MAX_CLIENTS = 10;
const int BUFFER_SIZE = 1024;
const int PORT = 8888;

int main() {
    int serverSocket, newSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrSize = sizeof(clientAddr);
    char buffer[BUFFER_SIZE];

    // Create socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Set up server address structure
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Error binding socket");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(serverSocket, MAX_CLIENTS) == -1) {
        perror("Error listening for connections");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    std::vector<pollfd> fds(MAX_CLIENTS + 1);
    fds[0].fd = serverSocket;
    fds[0].events = POLLIN;

    for (int i = 1; i <= MAX_CLIENTS; ++i) {
        fds[i].fd = -1;
    }

    while (true) {
        int activity = poll(fds.data(), MAX_CLIENTS + 1, -1);

        if (activity == -1) {
            perror("Error in poll");
            continue;
        }

        // Check for incoming connections
        if (fds[0].revents & POLLIN) {
            if ((newSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrSize)) == -1) {
                perror("Error accepting connection");
                continue;
            }

            // Add new connection to the list
            for (int i = 1; i <= MAX_CLIENTS; ++i) {
                if (fds[i].fd == -1) {
                    fds[i].fd = newSocket;
                    fds[i].events = POLLIN;
                    std::cout << "New connection accepted from " << inet_ntoa(clientAddr.sin_addr) << std::endl;
                    break;
                }
            }

            if (--activity <= 0) {
                continue;
            }
        }

        // Handle data from clients
        for (int i = 1; i <= MAX_CLIENTS; ++i) {
            if (fds[i].fd != -1 && (fds[i].revents & (POLLIN | POLLHUP))) {
                int bytesRead = recv(fds[i].fd, buffer, sizeof(buffer), 0);

                if (bytesRead <= 0) {
                    std::cout << "Connection closed by client" << std::endl;
                    close(fds[i].fd);
                    fds[i].fd = -1;
                } else {
                    buffer[bytesRead] = '\0';
                    std::cout << "Received data from client: " << buffer << std::endl;
                }

                if (--activity <= 0) {
                    break;
                }
            }
        }
    }

    // Close the server socket
    close(serverSocket);

    return 0;
}
