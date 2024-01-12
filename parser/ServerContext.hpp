/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerContext.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakebli <nakebli@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/31 19:57:19 by nakebli           #+#    #+#             */
/*   Updated: 2024/01/04 12:17:56 by nakebli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "servIO.hpp"
#include "MainContext.hpp"
#include <arpa/inet.h>

class MainContext;
class LocationContext;
class HttpContext;
class LogStream;
class ErrorPage;
class ListenAddress;
class HttpMethods;

class   Socket {
    public:
        Socket( int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0);
        ~Socket( void );
        void    bind( const sockaddr *addr, socklen_t addrlen );
        void    listen( int backlog );
        int     accept( sockaddr *addr, socklen_t *addrlen );
        void    close( void );
        int     getFd( void ) const;
    private:
        int         __fd;
        bool        __good;
        // sockaddr_in __addr;
        // socklen_t   __addrlen;
};

class	ServerContext : public HttpContext {
	public:
		std::vector<ListenAddress>					listenAddrs;
		HttpMethods									allowedMethods;
		std::map<std::string, LocationContext*>		locations;
        Socket                   					serverSocket;

		ServerContext( LogStream& lgs, ErrorPage& errPages ) : HttpContext(lgs, errPages) {};
		ServerContext( const HttpContext& http ) : HttpContext( http ) {};
		void	bindsocket( void );
        int    init_listenaddrs( void );
		~ServerContext( void );
};
