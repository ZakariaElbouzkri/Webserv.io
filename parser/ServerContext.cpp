/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerContext.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakebli <nakebli@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 11:06:00 by nakebli           #+#    #+#             */
/*   Updated: 2024/01/06 11:46:59 by nakebli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerContext.hpp"

ServerContext::~ServerContext( void ) {
    std::map<std::string, LocationContext*>::iterator it;
    for ( it = locations.begin(); it != locations.end(); it++ )
        delete it->second;
}

bool	ServerContext::has( const std::string& path ) {
    std::map<std::string, LocationContext*>::iterator it;
    it = locations.find( path );
    if ( it != locations.end() )
        return true;
    return false;
}
