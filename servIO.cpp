/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   servIO.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakebli <nakebli@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 12:40:18 by zel-bouz          #+#    #+#             */
/*   Updated: 2024/01/06 15:59:25 by nakebli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "servIO.hpp"
#include "ServerContext.hpp"

LogStream	logs;
ErrorPage	erPages;

#define ERROR404 "<h1> 404 - Not found </h1>"

int main() {
    // ServerContext serverContext(logs, erPages);
	MainContext main(logs, erPages);
	try {
		Parser parser;
		parser.parse(main);
	} catch (std::exception &e) {
		std::cerr << e.what() << '\n';
		return (EXIT_FAILURE);
	}
	// for (std::map<std::string, ServerContext *>::iterator it = main.servers.begin(); it != main.servers.end(); it++) {
	// 	// it->second->();
	// 	std::cout << it->first << "   " << it->second->serverSocket.getFd() << std::endl;
	// }

    MainContext::init_bindsocket(main.servers);
	ServerContext serverContext = *main.servers.begin()->second;
	pollfd pollFds[50]; // You can adjust this array size based on your needs
    pollFds[0].fd = serverContext.serverSocket.getFd();
    pollFds[0].events = POLLIN;

    while (true) {
        int readySockets = poll(pollFds, 10 + 1, -1);

        if (readySockets < 0) {
            // Handle error
            serverContext.logs << ERROR << "Error in poll: " << strerror(errno) << END;
            break;
        }

        if (pollFds[0].revents & POLLIN) {
            sockaddr_in clientAddr;
            socklen_t clientAddrLen = sizeof(clientAddr);
            int clientSocket = serverContext.serverSocket.accept(reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrLen);

            if (clientSocket >= 0) {
                // Handle the new client connection
                serverContext.logs << ACCESS << "New client connected" << END;

                std::string htmlContent = "<html><head><title>Hello World</title></head><body><h1>Hello, World!</h1></body></html>";
        
                std::string httpResponse = "HTTP/1.1 200 OK\r\n";
                httpResponse += "Content-Type: text/html\r\n";
                httpResponse += "Content-Length: " + std::to_string(htmlContent.size()) + "\r\n";
                httpResponse += "\r\n";
                httpResponse += htmlContent;
                // Send the response
                send(clientSocket, httpResponse.c_str(), httpResponse.size(), 0);
                // Close the client socket, or you can continue handling more requests from the same client
                close(clientSocket);
                // TODO: Implement code to handle the new client socket
            }
        }
    }
}
