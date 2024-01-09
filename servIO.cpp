/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   servIO.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakebli <nakebli@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 12:40:18 by zel-bouz          #+#    #+#             */
/*   Updated: 2024/01/09 12:14:47 by nakebli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "servIO.hpp"


LogStream	logs;
ErrorPage	erPages;

#define ERROR404 "<h1> 404 - Not found </h1>"

/*
	sdsd / HTTP/1.1
	.....
*/

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

	std::cout << "----------------------\n";

	pollfd	pollfds[50];
	int		i = 0;
	if ( main.servers.size() == 0 ) {
		std::cout << "No server found\n";
		return ( EXIT_FAILURE );
	}
	std::map<int, ServerContext*>::iterator it = main.ports.begin();
	while ( it != main.ports.end() ) {
		// std::cout << "fd : " << it->first << std::endl;
		pollfds[i].fd = it->first;
		pollfds[i].events = POLLIN;
		i++;
		it++;
	}
	while (true) {
        int readySockets = poll(pollfds, 10 + 1, -1);

		ServerContext serverContext = *main.servers.begin()->second;
        if (readySockets < 0) {
            // Handle error
            serverContext.logs << ERROR << "Error in poll: " << strerror(errno) << END;
            break;
        }

		i = 0;
		while (!(pollfds[i].revents & POLLIN))
			i++;
		std::cout << "readySockets : " << readySockets << " fd[i] : " << pollfds[i].fd << std::endl;
		

        if (pollfds[i].revents & POLLIN) {
            sockaddr_in clientAddr;
            socklen_t clientAddrLen = sizeof(clientAddr);
			int clientSocket = accept(pollfds[i].fd, reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrLen);
			
            // int clientSocket = serverContext..accept(reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrLen);

            if (clientSocket >= 0) {
                serverContext.logs << ACCESS << "New client connected" << END;

                std::string htmlContent = "<html><head><title>Hello World</title></head><body><h1>Hello, World!</h1></body></html>";
        
                std::string httpResponse = "HTTP/1.1 200 OK\r\n";
                httpResponse += "Content-Type: text/html\r\n";
                httpResponse += "Content-Length: " + std::to_string(htmlContent.size()) + "\r\n";
                httpResponse += "\r\n";
                httpResponse += htmlContent;

                send(clientSocket, httpResponse.c_str(), httpResponse.size(), 0);

                // close(clientSocket);
                // TODO: Implement code to handle the new client socket
            } else {
				serverContext.logs << ERROR << "Error accepting new client: " << strerror(errno) << END;
			}
		}
    }
}
