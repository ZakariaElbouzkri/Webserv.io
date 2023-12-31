/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerContext.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakebli <nakebli@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/31 19:57:19 by nakebli           #+#    #+#             */
/*   Updated: 2023/12/31 22:23:10 by nakebli          ###   ########.fr       */
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
        Socket( int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0) : __good(true) {
            __fd = socket( domain, type, protocol );
            if ( __fd < 0 ) {
                logs << ERROR << "Error creating socket" << END;
                __good = false;
            }
        }
        ~Socket( void );
        void    bind( const sockaddr *addr, socklen_t addrlen );
        void    listen( int backlog );
        int     accept( sockaddr *addr, socklen_t *addrlen );
        void    close( void );
        int     getFd( void ) const;
    private:
        int         __fd;
        bool        _good;
        sockaddr_in __addr;
        socklen_t   __addrlen;
        
};

class	ServerContext : public HttpContext {
	public:
		std::vector<ListenAddress>					listenAddrs;
		HttpMethods									allowedMethods;
		std::map<std::string, LocationContext*>		locations;
		Socket										serverSocket();

		ServerContext( LogStream& lgs, ErrorPage& errPages ) : HttpContext(lgs, errPages) { serverSocket = -1; };
		ServerContext( const HttpContext& http ) : HttpContext( http ) { serverSocket = -1;};
		void	bindsocket( void );
		~ServerContext( void );
};
