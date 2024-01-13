/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MainContext.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-bouz <zel-bouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 22:52:05 by zel-bouz          #+#    #+#             */
/*   Updated: 2024/01/13 15:06:27 by zel-bouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "servIO.hpp"

class	HttpMethods {
	public:
		void	allow( const std::string& method );
		bool	has( const std::string& method );
		bool	operator()( const std::string& method );

		HttpMethods( void );
		~HttpMethods( void );

	private:
		std::vector<std::string>	__accepted;
		std::vector<std::string>	__allowed;
};

class	ErrorPage {
	public:
		ErrorPage( void );
		~ErrorPage( void );
		void				add( int code, const std::string& page );
		const std::string	operator()( int code, const std::string& message );
	private:
		std::map<int, std::string>	__pages;
};

typedef enum { ACCESS, ERROR, END } stream_t;
class	LogStream {
	public:
		LogStream( void );
		~LogStream( void );

		void	setAccess( const std::string& acces );
		void	setError( const std::string& err );
	
		void	flush( void );
		LogStream&	operator<<(stream_t strm);

		template<typename T>
		LogStream&	operator<<(const T& data) {
			if ( !__errorStream.is_open() || !__accesStream.is_open() )
				this->openLogs();
			if ( __appendToAccess )
				__tempAccess << data;
			else
				__tempError << data;
			return *this;
		}			
	
	private:
		std::ofstream		__errorStream;
		std::ofstream		__accesStream;
		std::string			__access;
		std::string			__error;
		std::stringstream	__tempAccess;
		std::stringstream	__tempError;
		bool				__appendToAccess;
		
		void	__flushError( void );
		void	__flushAccess( void );
		void	openLogs( void );
};

class	ListenAddress {
	private:
		sockaddr_in	__addr;
		socklen_t	__len;
	public:
		ListenAddress( int port = 54000, int family = AF_INET, in_addr_t addr = INADDR_ANY );
		~ListenAddress( void );
		void	Length( socklen_t len );
		sockaddr_in&	Addr( void );
		socklen_t&		Len( void );
};


class	HttpContext {
	public:
		LogStream&					logs;
		ErrorPage					errorPage;
		std::vector<std::string>	index;
		bool						autoIndex;
		std::string					root;
		int							maxBody;

		HttpContext( LogStream& lgs ) : logs(lgs) {};
		~HttpContext( void ) {};
		HttpContext( const HttpContext& rhs ) : logs(rhs.logs) {
			this->errorPage = rhs.errorPage;
			this->index = rhs.index;
			this->autoIndex = rhs.autoIndex;
			this->root = rhs.root;
			this->maxBody = rhs.maxBody;
		};
		
};

class	LocationContext : public HttpContext {
	public:
		std::pair<int, std::string>				redirect;
		std::vector<std::string>				cgiExtentions;
		HttpMethods								allowedMethods;
		std::map<std::string, LocationContext*>	locations;

		LocationContext( LogStream& lgs ) : HttpContext(lgs) {};
		LocationContext( const HttpContext& http ) : HttpContext( http ) {};
		~LocationContext( );
		bool	has( const std::string& path );
};


class	ServerContext : public HttpContext {
	public:
		std::vector<ListenAddress>					listenAddrs;
		HttpMethods									allowedMethods;
		std::map<std::string, LocationContext*>		locations;
		std::vector<Socket>							sockets;

		ServerContext( LogStream& lgs ) : HttpContext(lgs) {};
		ServerContext( const HttpContext& http ) : HttpContext( http ) {};
		~ServerContext( void );

		void	createSocket(std::map<int, ServerContext&>&	ports, Poller& pollfds );
		bool	has( const std::string& path );
};

class	MainContext : public HttpContext {
	public:
		std::map<std::string, ServerContext*>	servers;
		std::map<int, ServerContext&>			ports;
		MainContext( LogStream& lgs );
		void createServerSockets( Poller& pollfds );
		// void addSocketToPoll( pollfd* pollfds );
		int getFd( pollfd *pollfds );
		~MainContext( void );
};
