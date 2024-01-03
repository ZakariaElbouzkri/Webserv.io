/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MainContext.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakebli <nakebli@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 22:52:05 by zel-bouz          #+#    #+#             */
/*   Updated: 2024/01/02 14:21:23 by nakebli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "servIO.hpp"
// #include "ServerContext.hpp"

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
		void				insert( std::vector<std::string> codes );
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
	public:
		void		setPort( const int& port );
		void		setHost( const std::string& host );
	
		const int&			getPort( void ) const;
		const std::string&	getHost( void ) const;
		ListenAddress( void );
		~ListenAddress( void );
	private:
		int			__port;
		std::string	__host;
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
};

class	MainContext : public HttpContext {
	public:
		std::map<std::string, ServerContext*>	servers;
		MainContext( LogStream& lgs, ErrorPage& errPages ) : HttpContext(lgs, errPages) {};
		~MainContext( void );
};


