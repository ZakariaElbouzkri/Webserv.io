/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MainContext.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakebli <nakebli@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 22:52:08 by zel-bouz          #+#    #+#             */
/*   Updated: 2024/01/02 14:21:28 by nakebli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "servIO.hpp"
#include "ServerContext.hpp"

/* httpMethods class */

HttpMethods::HttpMethods( void ){
	std::string accMethods[] = {
		"POST", "DELETE", "GET", // ....
	};
	size_t		size = sizeof(accMethods) / sizeof(accMethods[0]);
	__accepted = std::vector<std::string>(accMethods, accMethods + size);
}

HttpMethods::~HttpMethods( void ) {
}

void	HttpMethods::allow( const std::string& method ) {
	std::vector<std::string>::iterator it;
	it = std::find( __accepted.begin(), __accepted.end(), method );
	if ( it != __accepted.end() )
		__allowed.push_back( *it );
}

bool	HttpMethods::has( const std::string& method ) {
	std::vector<std::string>::iterator it;
	it = std::find( __allowed.begin(), __allowed.end(), method );
	if ( it != __allowed.end() )
		return true;
	return false;	
}

bool	HttpMethods::operator()( const std::string& method ) {
	return this->has( method );
}

/* ErrorPage class */

ErrorPage::ErrorPage( void ) {
}

ErrorPage::~ErrorPage( void ) {
}

void	ErrorPage::add( int code, const std::string& page ) {
	if ( code >= 100 && code <= 599 )
		__pages[code] = page;
}

const	std::string	ErrorPage::operator()( int code, const std::string& defaultPage ) {
	std::string		ans;
	std::map<int, std::string>::iterator it;
	it = __pages.find( code );
	if ( it != __pages.end() ) {
		std::ifstream	file( it->second );
		if (file.is_open() && std::getline( file, ans, '\0' ))
			return ans;
	}
	return defaultPage;
}

void	ErrorPage::insert( std::vector<std::string> codes ) {
	std::string	page = codes.back();
	for ( size_t i = 0; i < codes.size() - 1; i++ ) {
		std::stringstream	ss;
		ss << codes[i];
		int code; ss >> code;
		this->add( code, page );
	}
}

/* LogStream class */

LogStream::LogStream( void ) {
	__access = "./logs/access_log";
	__error = "./logs/error_log";
	__appendToAccess = true;
}


void	LogStream::openLogs( void ) {
	if ( !__errorStream.is_open() )
		__errorStream.open( __error.c_str(), std::ios::out | std::ios::app );
	if ( !__accesStream.is_open() )
		__accesStream.open( __access.c_str(), std::ios::out | std::ios::app );

	if ( !__errorStream.is_open() && !__appendToAccess )
		std::cerr << MAGENTA << "[ warning ]: " << RESET << "open " << __error << " for logging errors" << std::endl;
	if ( !__accesStream.is_open() && __appendToAccess )
		std::cerr << MAGENTA << "[ warning ]: " << RESET << "open " << __access << " for logging access" << std::endl;
}

LogStream::~LogStream( void ) {
	this->flush();
	__accesStream.close();
	__errorStream.close();
}


void	LogStream::setAccess( const std::string& acces ) {
	if ( strim( acces ).size() > 0 )
		__access = acces;
}

void	LogStream::setError( const std::string& err ) {
	if ( strim( err ).size() > 0 )
		__error = err;
}

void	LogStream::__flushAccess( void ) {
	if ( __accesStream.is_open() ) {
		__accesStream << __tempAccess.str();
		__accesStream.flush();
	} else {
		std::cout << __tempAccess.str();
		std::cout.flush();
	}
	__tempAccess.str("");
}

void	LogStream::__flushError( void ) {
	if ( __errorStream.is_open() ) {
		__errorStream << __tempError.str();
		__errorStream.flush();
	} else {
		std::cerr << __tempError.str();
		std::cerr.flush();
	}
	__tempError.str("");
}

void	LogStream::flush( void ) {
	__flushAccess();
	__flushError();
}

LogStream&	LogStream::operator<<( stream_t strm ) {
	if ( strm == END ) {
		if ( __appendToAccess )
			__tempAccess << '\n'; 
		else 
			__tempError << '\n';
		this->flush();
	} else {
		__appendToAccess = (strm == ACCESS);
	}
	return *this;
}


ListenAddress::ListenAddress( void ) {
}

ListenAddress::~ListenAddress( void ) {
}

const int&	ListenAddress::getPort( void ) const {
	return __port;
}

const std::string&	ListenAddress::getHost( void ) const {
	return __host;
}

void	ListenAddress::setPort( const int& port ) {
	__port = port;
}

void	ListenAddress::setHost( const std::string& host ) {
	__host = host;
}

LocationContext::~LocationContext( void ) {
	std::map<std::string, LocationContext*>::iterator it;
	it = this->locations.begin();
	for ( ; it != this->locations.end(); it++ ) {
		delete it->second;
	}
}


/*	MainContext class */

MainContext::~MainContext( void ) {
	std::map<std::string, ServerContext*>::iterator it;
	it = this->servers.begin();
	for ( ; it != this->servers.end(); it++) {
		delete it->second;
	}
}

