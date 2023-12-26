/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakebli <nakebli@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/25 21:56:23 by nakebli           #+#    #+#             */
/*   Updated: 2023/12/26 05:37:58 by nakebli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <map>
#include <fstream>
#include <sstream>

class Server
{
    private:
        int serverSocket;
        sockaddr_in serverAddress;
        sockaddr_in clientAddress;
        socklen_t clientAddressLength;
        std::map<std::string, int> clientMap;
    public:
        Server();
        ~Server();
        // void    createSocket();
        // void    bindSocket();
        // void    listenSocket();
        void    acceptSocket();
        void    handleCommunication(int clientSocket);
};

#endif