/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerContext.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakebli <nakebli@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/31 20:21:38 by nakebli           #+#    #+#             */
/*   Updated: 2024/01/04 18:33:27 by nakebli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerContext.hpp"





// ------------------------------ Socket ------------------------------ //

Socket::~Socket( void ) {
    if ( __good )
        ::close( __fd );
}

Socket::Socket( int domain, int type, int protocol ) : __good(true) {
    __fd = socket( domain, type, protocol );
    if ( __fd < 0 ) {
        logs << ERROR << "Error creating socket" << END;
        __good = false;
    }
    fcntl( __fd, F_SETFL, O_NONBLOCK );            
    printf("Socket constructor = %d\n", __fd);
}

void    Socket::bind( const sockaddr *addr, socklen_t addrlen ) {
    if ( ::bind( __fd, addr, addrlen ) < 0 ) {
        logs << ERROR << "Error binding socket" << END;
        __good = false;
    }
    else
    {
        logs << ACCESS << "Socket binded" << END;
        __good = true;
    }
}

void    Socket::listen( int backlog ) {
    if ( ::listen( __fd, backlog ) < 0 ) {
        logs << ERROR << "Error listening" << END;
        __good = false;
    }
    else
    {
        logs << ACCESS << "Socket listening" << END;
        __good = true;
    }
}

int     Socket::accept( sockaddr *addr, socklen_t *addrlen ) {
    int fd = ::accept( __fd, addr, addrlen );
    if ( fd < 0 ) {
        logs << ERROR << "Error accepting connection" << END;
        __good = false;
    }
    else
    {
        logs << ACCESS << "Connection accepted" << END;
        __good = true;
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




// ------------------------------ ServerContext ------------------------------ //

ServerContext::~ServerContext( void ) {
	std::map<std::string, LocationContext*>::iterator it;
	it = this->locations.begin();
	for ( ; it != this->locations.end(); it++ ) {
		delete it->second;
	}
    serverSocket.close();
}

void ServerContext::bindsocket() {
    for (std::vector<ListenAddress>::const_iterator it = listenAddrs.begin(); it != listenAddrs.end(); it++) {

        sockaddr_in addr;

        std::memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(8080);

        // if (inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr) <= 0) {
        //     logs << ERROR << "Invalid address: " << "127.0.0.1" << END;
        //     continue ;
        // }

        serverSocket.bind(reinterpret_cast<sockaddr *>(&addr), sizeof(addr));
        // serverSocket.listen(7); // Adjust the backlog value as needed
    }
}

int    ServerContext::init_listenaddrs( void ) {
    for ( int i = 8000;  i < 8003;  i++ ) {

        ListenAddress *it = new ListenAddress();
        it->setPort(i);
        it->setHost("127.0.0.1");
        this->listenAddrs.push_back(*it);
    }
    return 1;
}
