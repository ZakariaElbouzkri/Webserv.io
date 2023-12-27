/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MainContext.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-bouz <zel-bouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 14:34:01 by zel-bouz          #+#    #+#             */
/*   Updated: 2023/12/27 20:24:23 by zel-bouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "servIO.hpp"

// typedef std::map<int, std::string>	ErrorPage;
typedef std::vector<std::string>	Indexes;
typedef std::vector<std::string>	CgiAsign;

class	ErrorPage : public std::map<int, std::string> {
	public:
		ErrorPage( void ) {};
		~ErrorPage( void ) {};
};

class	ListenAddress{
	public:
		std::string	host;
		int			port;
		ListenAddress( void ) {};
		~ListenAddress( void ) {};
};

class	RedirectPage {
	public:
		int			code;
		std::string	path;
		
		RedirectPage( void ) {};
		~RedirectPage( void ) {};
};

class	HttpMethods : public std::set<std::string> {
	public:
		HttpMethods() {};
		~HttpMethods() {};
		bool	has( std::string key ) {
			if (this->find(key) != this->end())
				return true;
			return (false);
		}
};

class	LogStream {
	public:
		std::string	err;
		std::string	acces;

		LogStream( void ) {};
		~LogStream( void ) {};
};

class	HttpContext {
	public:
		ErrorPage		errorPages;
		HttpMethods		allowedMethods;
		Indexes			indexes;
		std::string		root;
		LogStream		logs;
		int				clientMaxBody;
		bool			autoIndex;
		HttpContext( void ) {};
		~HttpContext( void ) {};
};

class	LocationContext : public HttpContext {
	public:
		CgiAsign								cgis;
		RedirectPage							page;
		std::map<std::string, LocationContext*>	locations;
	
		LocationContext( void ) {};
		LocationContext( const HttpContext& rhs ) : HttpContext( rhs ) {};
		~LocationContext( void ) {};
};

class	ServerContext : public HttpContext {
	public:
		std::vector<ListenAddress>	listen;
		std::map<std::string, LocationContext*>	locations;

		ServerContext( void ) {};
		ServerContext( const HttpContext& rhs ) : HttpContext(rhs) {};
		~ServerContext( void ) { };		
};


class	MainContext : public HttpContext {
	public:
	
		std::map<std::string, ServerContext*>	servers;
		
		typedef	std::map<std::string, ServerContext*>::iterator	servIter;
		typedef std::map<std::string, LocationContext*>::iterator locIter;
	
		ServerContext&	operator[]( const std::string& key){
			return *servers.at(key);
		} ////////
		MainContext( void ) {};
		~MainContext( void ) {};
};
