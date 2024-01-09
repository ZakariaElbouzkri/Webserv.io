/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MainContext.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakebli <nakebli@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 22:52:05 by zel-bouz          #+#    #+#             */
/*   Updated: 2024/01/09 12:11:33 by nakebli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "servIO.hpp"
#include "ServerContext.hpp"

class ServerContext;

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
		ErrorPage&					errorPage;
		std::vector<std::string>	index;
		bool						autoIndex;
		std::string					root;
		int							maxBody;

		HttpContext( LogStream& lgs, ErrorPage& errPages ) : logs(lgs), errorPage(errPages) {};
		~HttpContext( void ) {};
		HttpContext( const HttpContext& rhs ) : logs(rhs.logs), errorPage(rhs.errorPage) {
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

		LocationContext( LogStream& lgs, ErrorPage& errPages ) : HttpContext(lgs, errPages) {};
		LocationContext( const HttpContext& http ) : HttpContext( http ) {};
		~LocationContext( );
		bool	has( const std::string& path );
};

class   Socket {
    public:
        Socket( int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0);
        ~Socket( void ) { };
        bool    bind( const sockaddr *addr, socklen_t addrlen );
        bool    listen( int backlog );
        int     accept( sockaddr *addr, socklen_t *addrlen );
        void    close( void );
        int     getFd( void ) const;
		bool    good( void ) const {
			return __good;
		}
    private:
        int         __fd;
        bool        __good;
};

class	ServerContext : public HttpContext {
	public:
		std::vector<ListenAddress>					listenAddrs;
		HttpMethods									allowedMethods;
		std::map<std::string, LocationContext*>		locations;
		std::vector<Socket>							sockets;

		ServerContext( LogStream& lgs, ErrorPage& errPages ) : HttpContext(lgs, errPages) {};
		ServerContext( const HttpContext& http ) : HttpContext( http ) {};
		~ServerContext( void );

		void	createSocket(std::map<int, ServerContext*>& ports);
		bool	has( const std::string& path );
};

class	MainContext : public HttpContext {
	public:
		std::map<std::string, ServerContext*>	servers;
		std::map<int, ServerContext*>			ports;
		MainContext( LogStream& lgs, ErrorPage& errPages );
		void createServerSockets( void );
		~MainContext( void );
};
