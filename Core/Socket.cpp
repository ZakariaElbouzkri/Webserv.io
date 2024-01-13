/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-bouz <zel-bouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 10:58:18 by zel-bouz          #+#    #+#             */
/*   Updated: 2024/01/13 10:59:52 by zel-bouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "servIO.hpp"


Socket::Socket( int domain, int type, int protocol ) : __good(true) {
    __fd = socket( domain, type, protocol );
    if ( __fd < 0 ) {
        // logs << ERROR << "Error creating socket" << END;
        __good = false;
    }
    fcntl( __fd, F_SETFL, O_NONBLOCK | O_APPEND); 
    // printf("Socket constructor = %d\n", __fd);
}

bool    Socket::bind( const sockaddr *addr, socklen_t addrlen ) {
    if ( ::bind( __fd, addr, addrlen ) < 0 ) {
        logs << ERROR << "Error binding socket" << END;
        return false;
    }
	logs << ACCESS << "Socket binded" << END;
	return true;
}

bool    Socket::listen( int backlog ) {
    if ( ::listen( __fd, backlog ) < 0 ) {
        logs << ERROR << "Error listening" << END;
		return false;
    }
	logs << ACCESS << "Socket listening" << END;
	return true;
}

int     Socket::accept( sockaddr *addr, socklen_t *addrlen ) {
    int fd = ::accept( __fd, addr, addrlen );
    return fd;
}

void    Socket::close( void ) {
    if ( __good )
        ::close( __fd );
}

int     Socket::getFd( void ) const {
    return __fd;
}

bool    Socket::good( void ) const {
	return __fd;
}

