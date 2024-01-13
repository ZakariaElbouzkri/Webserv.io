/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-bouz <zel-bouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 10:57:45 by zel-bouz          #+#    #+#             */
/*   Updated: 2024/01/13 10:59:34 by zel-bouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "servIO.hpp"

class   Socket {
    public:
        Socket( int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0);
        ~Socket( void ) { };
        bool    bind( const sockaddr *addr, socklen_t addrlen );
        bool    listen( int backlog );
        int     accept( sockaddr *addr, socklen_t *addrlen );
        void    close( void );
        int     getFd( void ) const;
		bool    good( void ) const;
    private:
        int         __fd;
        bool        __good;
};
