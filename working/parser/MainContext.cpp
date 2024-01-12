/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MainContext.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakebli <nakebli@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 22:52:08 by zel-bouz          #+#    #+#             */
/*   Updated: 2024/01/12 12:05:03 by nakebli          ###   ########.fr       */
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








/* Address */

ListenAddress::ListenAddress( int port, int family, in_addr_t addr) {
	__addr.sin_family = family;
	__addr.sin_port = htons( port );
	__addr.sin_addr.s_addr = addr;
	__len = sizeof( __addr );
}

ListenAddress::~ListenAddress( void ) {
}

void	ListenAddress::Length( socklen_t len ) {
	__len = len;
}

socklen_t&	ListenAddress::Len( void ) {
	return __len;
}

sockaddr_in&	ListenAddress::Addr( void ) {
	return __addr;
}



LocationContext::~LocationContext( void ) {
	std::map<std::string, LocationContext*>::iterator it;
	it = this->locations.begin();
	for ( ; it != this->locations.end(); it++ ) {
		delete it->second;
	}
}

bool	LocationContext::has( const std::string& path ) {
	if ( this->locations.empty() )
		return false;
	std::map<std::string, LocationContext*>::iterator it = this->locations.begin();
	std::map<std::string, LocationContext*>::iterator itEnd = this->locations.end();
	while ( it != itEnd ) {
		if ( it->first == path || it->second->has( path ) )
			return true;
		it++;
	}
	return false;
}










/*	ServerContext class */

bool	ServerContext::has( const std::string& path ) {
	if ( this->locations.empty() )
		return false;
	std::map<std::string, LocationContext*>::iterator it = this->locations.begin();
	std::map<std::string, LocationContext*>::iterator itEnd = this->locations.end();
	while ( it != itEnd ) {
		if ( it->first == path || it->second->has( path ) )
			return true;
		it++;
	}
	return false;
}

ServerContext::~ServerContext( void ) {
	// std::map<std::string, LocationContext*>::iterator it;
	// it = this->locations.begin();
	// for ( ; it != this->locations.end(); it++ ) {
	// 	delete it->second;
	// }
}

void	ServerContext::createSocket(std::map<int, ServerContext&>&	ports)
{
	std::vector<ListenAddress>::iterator itb = this->listenAddrs.begin();
	std::vector<ListenAddress>::iterator ite = this->listenAddrs.end();
	while (itb != ite) {
		Socket sock(AF_INET, SOCK_STREAM, 0);
		if (!sock.good()) {
			itb++;
			continue;
		}
		if (!sock.bind((sockaddr*)&itb->Addr(), itb->Len()))  {
			itb++;
			continue;
		}
		if (!sock.listen(100)) {
			itb++;
			continue;
		}
		ports.insert(std::pair<int, ServerContext&>(sock.getFd(), *this));
		itb++;
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

void   MainContext::createServerSockets( void ) {
	std::map<std::string, ServerContext*>::iterator itb = this->servers.begin();
	std::map<std::string, ServerContext*>::iterator ite = this->servers.end();
	while (itb != ite) {
		itb->second->createSocket(this->ports);
		itb++;
	}
}

void MainContext::addSocketToPoll( pollfd* pollfds ) {
	int i = 0;
	std::map<int, ServerContext&>::const_iterator it = this->ports.begin();
	while ( it != this->ports.end() ) {
		pollfds[i].fd = it->first;
		pollfds[i].events = POLLIN | POLLOUT;
		i++;
		it++;
	}
}

int MainContext::getFd( pollfd *pollfds ) {
	int readySockets = poll(pollfds, 10 + 1, -1);

	if (readySockets < 0) {
		// Handle error
		this->logs << ERROR << "Error in poll: " << strerror(errno) << END;
		return -1;
	}
	else if (readySockets == 0) {
		// Handle timeout (not possible here, but good practice)
		this->logs << ERROR << "Timeout in poll" << END;
		return -1;
	}
	int i = 0;
	while (!(pollfds[i].revents & POLLIN) && !(pollfds[i].revents & POLLOUT))
		i++;
	return i;
}

MainContext::MainContext( LogStream& lgs, ErrorPage& errPages ) : HttpContext(lgs, errPages) {
}


/*    Socket    */

Socket::Socket( int domain, int type, int protocol ) : __good(true) {
    __fd = socket( domain, type, protocol );
    if ( __fd < 0 ) {
        // logs << ERROR << "Error creating socket" << END;
        __good = false;
    }
    fcntl( __fd, F_SETFL, O_NONBLOCK );            
    // printf("Socket constructor = %d\n", __fd);
}

bool    Socket::bind( const sockaddr *addr, socklen_t addrlen ) {
    if ( ::bind( __fd, addr, addrlen ) < 0 ) {
        logs << ERROR << "Error binding socket" << END;
        return false;
    }
	logs << ACCESS << "Socket binded" << END;
	return true;
}

bool    Socket::listen( int backlog ) {
    if ( ::listen( __fd, backlog ) < 0 ) {
        logs << ERROR << "Error listening" << END;
		return false;
    }
	logs << ACCESS << "Socket listening" << END;
	return true;
}

int     Socket::accept( sockaddr *addr, socklen_t *addrlen ) {
    int fd = ::accept( __fd, addr, addrlen );
    return fd;
}

void    Socket::close( void ) {
    if ( __good )
        ::close( __fd );
}

int     Socket::getFd( void ) const {
    return __fd;
}
