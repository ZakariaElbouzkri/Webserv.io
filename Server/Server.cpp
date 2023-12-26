/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakebli <nakebli@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/25 21:58:04 by nakebli           #+#    #+#             */
/*   Updated: 2023/12/26 05:55:57 by nakebli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server()
{
    this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->serverSocket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        exit(1);
    }
    this->serverAddress.sin_family = AF_INET;
    this->serverAddress.sin_addr.s_addr = INADDR_ANY; // Use any available network interface
    this->serverAddress.sin_port = htons(8080); // Use port 8080
    if (bind(this->serverSocket, (struct sockaddr*)&this->serverAddress, sizeof(this->serverAddress)) == -1) {
        std::cerr << "Error binding socket" << std::endl;
        close(this->serverSocket);
        exit(1);
    }
    if (listen(this->serverSocket, 10) == -1) {
        std::cerr << "Error listening for connections" << std::endl;
        close(this->serverSocket);
        exit(1);
    }
    std::cout << "Server is listening on port 8080..." << std::endl;
}

Server::~Server()
{
    close(this->serverSocket);
}

void    Server::acceptSocket()
{
    while (true)
    {
        this->clientAddressLength = sizeof(this->clientAddress);
        int clientSocket = accept(this->serverSocket, (struct sockaddr *)&this->clientAddress, &this->clientAddressLength);
        if (clientSocket == -1)
        {
            std::cerr << "Error accepting connection" << std::endl;
            continue;
        }

        std::string clientIp = inet_ntoa(this->clientAddress.sin_addr);
        std::cout << "Connection accepted from " << clientIp << ":" << ntohs(this->clientAddress.sin_port) << std::endl;

        // Add the client socket and IP address to the map
        this->clientMap[clientIp] = clientSocket;

        // Handle communication for the accepted client
        this->handleCommunication(clientMap[clientIp]);
    }
}

void    Server::handleCommunication(int clientSocket)
{
    char buffer[1024];
    int bytesReceived = recv(clientSocket, buffer, 1024, 0);
    if (bytesReceived == -1) {
        std::cerr << "Error in recv(). Quitting" << std::endl;
        close(clientSocket);
        exit(1);
    }
    if (bytesReceived == 0) {
        std::cout << "Client disconnected " << std::endl;
        close(clientSocket);
        exit(1);
    }
    // std::cout << std::string(buffer, 0, bytesReceived) << std::endl;
    // send(clientSocket, buffer, bytesReceived + 1, 0);

    std::ifstream htmlFile("example.html");
    std::stringstream buffer2;
    buffer2 << htmlFile.rdbuf();
    std::string htmlContent = buffer2.str();

    std::string httpResponse = "HTTP/1.1 200 OK\r\n";
    httpResponse += "Content-Type: text/html\r\n";
    httpResponse += "Content-Length: " + std::to_string(htmlContent.size()) + "\r\n";
    httpResponse += "\r\n";
    httpResponse += htmlContent;

    // Send HTML file content
    int bytesSent = send(clientSocket, httpResponse.c_str(), httpResponse.size(), 0);

    if (bytesSent == -1) {
        std::cerr << "Error sending data" << std::endl;
    } else {
        std::cout << "Sent " << bytesSent << " bytes to the client" << std::endl;
    }
}
