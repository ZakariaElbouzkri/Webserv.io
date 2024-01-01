/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerContext.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakebli <nakebli@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/31 20:21:38 by nakebli           #+#    #+#             */
/*   Updated: 2024/01/01 17:52:19 by nakebli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerContext.hpp"


Socket::~Socket( void ) {
    if ( __good )
        ::close( __fd );
}

void    Socket::bind( const sockaddr *addr, socklen_t addrlen ) {
    if ( ::bind( __fd, addr, addrlen ) < 0 ) {
        logs << ERROR << "Error binding socket" << END;
        __good = false;
    }
}

void    Socket::listen( int backlog ) {
    if ( ::listen( __fd, backlog ) < 0 ) {
        logs << ERROR << "Error listening" << END;
        __good = false;
    }
}

int     Socket::accept( sockaddr *addr, socklen_t *addrlen ) {
    int fd = ::accept( __fd, addr, addrlen );
    if ( fd < 0 ) {
        logs << ERROR << "Error accepting connection" << END;
        __good = false;
    }
    return fd;
}

void    Socket::close( void ) {
    if ( __good )
        ::close( __fd );
}

int     Socket::getFd( void ) const {
    return __fd;
}

ServerContext::~ServerContext( void ) {
	std::map<std::string, LocationContext*>::iterator it;
	it = this->locations.begin();
	for ( ; it != this->locations.end(); it++ ) {
		delete it->second;
	}
}

void	ServerContext::bindsocket( void ) {
    struct sockaddr_in serv_addr;
    int opt = 1;

    if (setsockopt(serverSocket.getFd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        std::cerr << "setsockopt" << std::endl;
        exit(1);
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(this->listenAddrs[0].getPort());
    if (bind(serverSocket.getFd(), (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Error binding socket" << std::endl;
        exit(1);
    }
    if (listen(serverSocket.getFd(), 10) < 0) {
        std::cerr << "Error listening" << std::endl;
        exit(1);
    }
}
