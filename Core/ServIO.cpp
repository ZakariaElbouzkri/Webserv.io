/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServIO.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakebli <nakebli@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 11:11:12 by zel-bouz          #+#    #+#             */
/*   Updated: 2024/01/13 21:00:32 by nakebli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "servIO.hpp"
// #include "Poller.hpp"

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
				ClientInfo client;
				if ( main.clients.find(serverPolls[i].fd) != main.clients.end() )
					client = main.clients[serverPolls[i].fd];
				else
				{
					client = ClientInfo(serverPolls[i].fd);
					if ( client.fd != -1 )
						main.clients.insert(std::pair<int, ClientInfo&>(client.fd,  client));
				}
				if ( client.fd == -1) {
					logs << ERROR << "failed to accept new client" << END;
				}
				logs << ACCESS << "new client connected" << END;
				clientPolls.pushFd(client.fd, POLLIN);
				std::cout << client.addr.sin_addr.s_addr << "  " << client.len << "  " << client.fd << std::endl;
			}
		}
		// class client in the code bellow still not set up correctly 
		int	j = clientPolls.poll();
		if ( j >= 0 ) {
			char	buffer[4096] = {0};
			ClientInfo client = main.clients[clientPolls[j].fd];
			int rd = recv(client.fd, buffer, client.request.size(), 0);
			if (rd == 0) {
				// clientPolls.erase(clientPolls[j].fd);
				// try {
				// 	main.clients.erase(clientPolls[j].fd);
				// }
				// catch (std::exception & e) {
				// 	logs << e.what() << END;
				// }
				continue;
			}
			else if ( rd < 0 )
				logs << ERROR << "Error recieving message from the client" << END;
			else
			{
				client.request += buffer;
				std::cout << "----------Received-------\n\n" << client.request << '\n';
			}
		}
    }
}
