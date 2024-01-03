/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   servIO.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakebli <nakebli@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 12:40:18 by zel-bouz          #+#    #+#             */
/*   Updated: 2024/01/02 12:42:00 by nakebli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "servIO.hpp"
#include "ServerContext.hpp"

LogStream	logs;
ErrorPage	erPages;

#define ERROR404 "<h1> 404 - Not found </h1>"

int main() {
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
	ServerContext serverContext = *main.servers.begin()->second;
	pollfd pollFds[50]; // You can adjust this array size based on your needs
    pollFds[0].fd = serverContext.serverSocket.getFd();
    pollFds[0].events = POLLIN;

    while (true) {
        int readySockets = poll(pollFds, 1, -1); // Wait indefinitely for events

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

                // TODO: Implement code to handle the new client socket
                // For example, you can create a new thread or process to handle the client.
            }
        }
    }
}
