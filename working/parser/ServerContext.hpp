/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerContext.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakebli <nakebli@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 11:06:31 by nakebli           #+#    #+#             */
/*   Updated: 2024/01/06 12:15:51 by nakebli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "servIO.hpp"
#include "MainContext.hpp"

class MainContext;
class LocationContext;
class HttpContext;
class LogStream;
class ErrorPage;
class ListenAddress;
class HttpMethods;

// class	ServerContext : public HttpContext {
// 	public:
// 		std::vector<ListenAddress>					listenAddrs;
// 		HttpMethods									allowedMethods;
// 		std::map<std::string, LocationContext*>		locations;

// 		ServerContext( LogStream& lgs, ErrorPage& errPages ) : HttpContext(lgs, errPages) {};
// 		ServerContext( const HttpContext& http ) : HttpContext( http ) {};
// 		~ServerContext( void );

// 		bool	has( const std::string& path );
// };