/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-bouz <zel-bouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 11:56:12 by zel-bouz          #+#    #+#             */
/*   Updated: 2023/12/30 06:16:23 by zel-bouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "servIO.hpp"

Parser::Parser( void ) : __currTok( Token::_EOF ) {
	if ( !__lexer.good() ) {
		throw FileNotFoundOrEmpty();
	}
	__currTok = __lexer.getNextToken();
}

Parser::~Parser( void ) {
}

void	Parser::__parseErrorPage( HttpContext& httpCtx ) {
	std::vector<std::string>	codes;
	__advance( Token::ERR_PAGE );
	while ( __currTok == Token::WORD ) {
		codes.push_back( __currTok.getData() );
		__advance( Token::WORD );
	}
	if ( codes.size() < 2)
		__advance( Token::WORD );
	httpCtx.errorPage.insert( codes );
}

void	Parser::__parseLogs( HttpContext& httpCtx ) {
	if ( __currTok == Token::ACC_LOG ) {
		__advance( Token::ACC_LOG );
		httpCtx.logs.setAccess( __currTok.getData() );
	} else	if ( __currTok == Token::ERR_LOG) {
		__advance( Token::ERR_LOG );
		httpCtx.logs.setError( __currTok.getData() );
	} else  if ( __currTok == Token::ROOT ) {
		__advance( Token::ROOT );
		httpCtx.root = __currTok.getData();
	} else {
		__advance( Token::MAX_BODY );
		httpCtx.maxBody = atoi( __currTok.getData().c_str() );
	}
	__advance( Token::WORD );
}

void	Parser::__parseMethods( HttpMethods& methods ) {
	__advance( Token::ALLOW );
	if ( __currTok != Token::WORD ) {
		__log << "expected allow methods found " << __currTok;
		__logError();
	}
	while ( __currTok == Token::WORD ) {
		methods.allow( __currTok.getData() );
		__advance( Token::WORD );
	}
}

void	Parser::__parseIndexes( HttpContext& httpCtx ) {
	if ( __currTok == Token::INDEX ) {
		__advance( Token::INDEX );
		if ( __currTok != Token::WORD )
			__advance( Token::WORD );
		while ( __currTok == Token::WORD ) {
			httpCtx.index.push_back( __currTok.getData() );
			__advance( Token::WORD );
		}
	} else {
		__advance( Token::AUTOINDEX );
		if ( __currTok.getData() == "on" ) // may be check if data in ["on", "off"] and throw err;
			httpCtx.autoIndex = true;
		__advance( Token::WORD );
	}
}

void	Parser::__parseHttpDirectives( HttpContext& httpCtx ) {
	switch ( __currTok.getType() ) {
		case Token::ERR_PAGE:	
			__parseErrorPage( httpCtx ); break;
		case Token::ERR_LOG:
		case Token::ROOT:
		case Token::MAX_BODY:
		case Token::ACC_LOG:
			__parseLogs( httpCtx ); break;
		case Token::AUTOINDEX:
		case Token::INDEX:
			__parseIndexes( httpCtx ); break;
		default:
			__log << "directive " << __currTok << " not in it's context";
			__logError();
	}
}


std::string	Parser::__parseKey( void ) {
	__advance( Token::OPEN_SQUARE );

	std::string	key = __currTok.getData();

	__advance( Token::WORD );

	__advance( Token::CLOSE_SQUARE );
	return key;
}


ListenAddress	Parser::__parseListenAddr( void ) {
	ListenAddress		adr;
	std::stringstream	ss;
	std::string			host;
	int					port;

	__advance( Token::LISTEN );
	size_t	pos = __currTok.getData().find(':');
	if ( pos != std::string::npos ) {
		ss << __currTok.getData().substr( 0, pos );
		ss << " " << __currTok.getData().substr( pos + 1);
		ss >> host;
		ss >> port;
	} else {
		ss << __currTok.getData();
		ss >> port;
	}
	__advance( Token::WORD );
	adr.setHost( host );
	adr.setPort( port );
	return adr;
}


void	Parser::__parseRedirectPage( std::pair<int, std::string>& page ) {
	std::stringstream	ss;
	__advance( Token::RETURN );
	ss << __currTok.getData();
	__advance( Token::WORD );
	ss << __currTok.getData();
	__advance( Token::WORD );
	
	ss >> page.first;
	ss >> page.second;
}

void	Parser::__parseCgiExt( std::vector<std::string>& cgi ) {
	__advance( Token::CGI );
	cgi.push_back( __currTok.getData() );
	__advance( Token::WORD );
	while ( __currTok == Token::WORD ) {
		cgi.push_back( __currTok.getData() );
		__advance( Token::WORD );
	}
}

LocationContext*	Parser::__parseLocation( HttpContext& httpCtx ) {
	__advance( Token::OPEN_CURLY );
	LocationContext* route = new LocationContext( httpCtx );

	while ( __currTok != Token::LOCATION && __currTok != Token::CLOSE_CURLY ) {
		switch ( __currTok.getType() ) {
			case Token::RETURN: __parseRedirectPage( route->redirect );		break;
			case Token::ALLOW:	__parseMethods( route->allowedMethods );	break;
			case Token::CGI:	__parseCgiExt( route->cgiExtentions );		break;
			default:			__parseHttpDirectives( *route );			break;
		}
		__advance( Token::SIMICOLEN );
	}
	
	while ( __currTok == Token::LOCATION ) {
		__advance( Token::LOCATION );
		std::string	locationPath = __parseKey();
		route->locations[locationPath] = __parseLocation( *route );
	}
	__advance( Token::CLOSE_CURLY );
	return route;
}

ServerContext*	Parser::__parseServer( HttpContext& httpCtx ) {
	__advance( Token::OPEN_CURLY );

	ServerContext* serv = new ServerContext( httpCtx );
	while ( __currTok != Token::LOCATION && __currTok != Token::CLOSE_CURLY ) {
		switch ( __currTok.getType() ) {
			case Token::LISTEN: serv->listenAddrs.push_back( __parseListenAddr( ) ); break;
			case Token::ALLOW:	__parseMethods( serv->allowedMethods ); break;;
			default:	__parseHttpDirectives( *serv ); break;	
		}
		__advance( Token::SIMICOLEN );
	}
	while ( __currTok == Token::LOCATION ) {
		__advance( Token::LOCATION );
		std::string	locationPath = __parseKey();
		serv->locations[locationPath] = __parseLocation( *serv );
	}
	__advance( Token::CLOSE_CURLY );
	return serv;
}

void	Parser::parse( MainContext& http ) {
	__advance( Token::HTTP );
	__advance( Token::OPEN_CURLY );
	while ( __currTok != Token::SERVER && __currTok != Token::CLOSE_CURLY ) {
		__parseHttpDirectives( http );
		__advance( Token::SIMICOLEN );
	}

	while ( __currTok == Token::SERVER ) {
		__advance( Token::SERVER );
		std::string	serverName = __parseKey();
		std::map<std::string, ServerContext*>::iterator it = http.servers.find(serverName);
		if ( it != http.servers.end()) {
			__log << "duplicated servername: " << serverName;
			__logError();
		}
		http.servers[serverName] = __parseServer( http );
	}
	__advance( Token::CLOSE_CURLY );
	__advance( Token::_EOF );
}

void	Parser::__logError( void ) {
	std::string	err;
	std::cerr << "webserv: configFile:\n" ;
	std::cerr << "\tat line: " << CYAN << __lexer.line << RESET << ' ' << __log.str();
	throw	SyntaxError();
}

void	Parser::__advance( Token::token_t tok ) {
	if ( __currTok != tok ) {
		__log << "expected " << Token(tok) << " but found " <<  __currTok;
		__logError();
	}
	if ( __currTok != Token::_EOF )
		__currTok = __lexer.getNextToken();
}