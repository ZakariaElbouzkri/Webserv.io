/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServIO.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-bouz <zel-bouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 11:11:12 by zel-bouz          #+#    #+#             */
/*   Updated: 2024/01/13 11:25:06 by zel-bouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "servIO.hpp"

void	servCore( MainContext& main ) {
	main.createServerSockets();
	pollfd	pollfds[50];
	if ( main.servers.size() == 0  || main.ports.size() == 0) {
		std::cerr << "No server found\n";
		exit ( EXIT_FAILURE );
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