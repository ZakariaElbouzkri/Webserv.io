/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServIO.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-bouz <zel-bouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 11:11:12 by zel-bouz          #+#    #+#             */
/*   Updated: 2024/01/13 15:45:24 by zel-bouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "servIO.hpp"
#include "Poller.hpp"

/*
	serverPolls;
	clientPolls;
*/


void	servCore( MainContext& main ) {
	Poller	clientPolls;
	Poller	serverPolls;
	
	main.createServerSockets( serverPolls );

	while (main.ports.size()) {
		int i = serverPolls.poll();
		if (i >= 0) {
			if ( serverPolls[i].revents & POLLIN ) {
				sockaddr_in clientAddr;
            	socklen_t clientAddrLen = sizeof(clientAddr);
				int	clientSocket = accept(serverPolls[i].fd,
					reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrLen);
				if ( clientSocket == -1) {
					logs << ERROR << "failed to accept new client" << END;
				}
				logs << ACCESS << "new client connected" << END;
				clientPolls.pushFd(clientSocket);
			}
		}
		int	j = clientPolls.poll();
		if ( j >= 0 ) {
			char	buffer[4096] = {0};
			int rd = recv(clientPolls[j].fd, buffer, sizeof(buffer), 0);
			if (rd == 0) {
				clientPolls.erase(clientPolls[j].fd);
				continue;
			}
			if ( rd < 0 )
				logs << ERROR << "Error recieving message from the client" << END;
			else
				std::cout << "Received: " << buffer << '\n';
		}
    }
}