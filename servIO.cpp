/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   servIO.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakebli <nakebli@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 12:40:18 by zel-bouz          #+#    #+#             */
/*   Updated: 2024/01/12 20:36:02 by nakebli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "servIO.hpp"


LogStream	logs;
ErrorPage	erPages;

#define ERROR404 "<h1> 404 - Not found </h1>"

int main() {
	MainContext main( logs, erPages );
	try {
		Parser		parser;
		parser.parse( main );
	} catch( std::exception& e) {
		std::cerr << e.what() << '\n';
		return ( EXIT_FAILURE );
	}
	main.createServerSockets();
	pollfd	pollfds[50];
	if ( main.servers.size() == 0  || main.ports.size() == 0) {
		std::cerr << "No server found\n";
		return ( EXIT_FAILURE );
	}
	main.addSocketToPoll ( pollfds );
	while (true) {
		int i = main.getFd( pollfds );
		if ( i < 0 )
			continue ;
        if (i < 50 && pollfds[i].revents & POLLIN ) {
			// std::cout << "POLLIN\n";
            sockaddr_in clientAddr;
            socklen_t clientAddrLen = sizeof(clientAddr);
			int clientSocket = accept(pollfds[i].fd, reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrLen);

			if (clientSocket >= 0) {
                logs << ACCESS << "New client connected" << END;

				char buffer[4096] = {0}; // Buffer to store the request
				// memset(buffer, 0, sizeof(buffer)); // Clear the buffer
				
				// Receive the request
				int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
				if (bytesReceived < 0)
				    std::cerr << "Error: " << strerror(errno) << std::endl;
				else {
				    std::cout << "Received: " << buffer << std::endl;
				}
            } else {
				logs << ERROR << "Error accepting new client: " << strerror(errno) << END;
			}
		}
    }
}


// if (i < 50 && pollfds[i].revents & POLLOUT) {
// 	std::cout << "POLLOUT\n";
//         std::string htmlContent = "<html><head><title>Hello World</title></head><body><h1>Hello, \
// 									World!</h1></body></html>";
//         std::string httpResponse = "HTTP/1.1 200 OK\r\n";
//         httpResponse += "Content-Type: text/html\r\n";
//         httpResponse += "Content-Length: " + std::to_string(htmlContent.size()) + "\r\n";
//         httpResponse += "\r\n";
//         httpResponse += htmlContent;

//         send(pollfds[i].fd, httpResponse.c_str(), httpResponse.size(), 0);
//         close(pollfds[i].fd);
// }
