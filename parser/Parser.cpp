/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-bouz <zel-bouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 11:56:12 by zel-bouz          #+#    #+#             */
/*   Updated: 2024/01/03 19:17:03 by zel-bouz         ###   ########.fr       */
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

	for ( size_t i = 0; i < codes.size() - 1; i++ ) {
		int	code;
		if ( !ft_stoi( codes[i], code ) || code < 100 || code > 599 ) {
			__log << "at line: " << __lexer.line << " invalid error code: " << codes[i];
			throw SyntaxError( __log );
		}
		httpCtx.errorPage.add( code, codes.back() );
	}
}

void	Parser::__parseLogs( HttpContext& httpCtx ) {
	switch ( __currTok.getType() ) {
		case Token::ACC_LOG:
			__advance( Token::ACC_LOG );
			httpCtx.logs.setAccess( __currTok.getData() );
			break;
		case Token::ERR_LOG:
			__advance( Token::ERR_LOG );
			httpCtx.logs.setError( __currTok.getData() );
			break;
		case Token::ROOT:
			__advance( Token::ROOT );
			httpCtx.root = __currTok.getData();
			break;
		default:
			__advance( Token::MAX_BODY );
			if ( !ft_stoi( __currTok.getData(), httpCtx.maxBody ) || httpCtx.maxBody <= 0 ) {
				__log << "at line " << __lexer.line << " invalid max body size number " << __currTok.getData();
				throw SyntaxError( __log );
			}
			break;
	}
	__advance( Token::WORD );
}

void	Parser::__parseMethods( HttpMethods& methods ) {
	__advance( Token::ALLOW );
	
	if ( __currTok != Token::WORD ) {
		__log << "at line: " << __lexer.line << " expected method name found " << __currTok;
		throw SyntaxError( __log );
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
		if ( __currTok.getData() != "off" && __currTok.getData() != "on" ) {
			__log << "at line " << __lexer.line << " expected value 'on' or 'off' found " << __currTok;
			throw SyntaxError( __log );
		}
		httpCtx.autoIndex = ( __currTok.getData() == "on" );
		__advance( Token::WORD );
	}
}

void	Parser::__parseHttpDirectives( HttpContext& httpCtx ) {
	switch ( __currTok.getType() ) {
		case Token::ERR_PAGE:	
			__parseErrorPage( httpCtx );
			break;
		case Token::ERR_LOG:
		case Token::ROOT:
		case Token::MAX_BODY:
		case Token::ACC_LOG:
			__parseLogs( httpCtx ); break;
		case Token::AUTOINDEX:
		case Token::INDEX:
			__parseIndexes( httpCtx ); break;
		default:
			__log << "unexpected token '" << __currTok << "' at line: " << __lexer.line;
			throw SyntaxError( __log );
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
	int	port;

	__advance( Token::LISTEN );
	if ( !ft_stoi( __currTok.getData(), port ) || port <= 0 || port > 65535 ) {
		__log << "at line: " << __lexer.line << " invalid port number: " << __currTok.getData();
		throw SyntaxError( __log );
	}
	__advance( Token::WORD );
	return ListenAddress( port );
}


void	Parser::__parseRedirectPage( std::pair<int, std::string>& page ) {
	__advance( Token::RETURN );
	if ( ft_stoi( __currTok.getData(), page.first ) == false ) {
		__log << "at line: " << __lexer.line << " invalid return code: " << __currTok.getData();
		throw SyntaxError( __log );
	}
	__advance( Token::WORD );
	page.second = __currTok.getData();
	__advance( Token::WORD );
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
	if ( serv->listenAddrs.size() == 0 ) {
		__log << "server block must contain at least one listen directive";
		throw SyntaxError( __log );
	}
	if ( serv->root.size() == 0 ) {
		__log << "root directive must be specified in [server] block .\n";
		__log << "you can also specify one in [http] block to be used as default root";
		throw SyntaxError( __log );
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
			__log << "server name " << serverName << " at line " << __lexer.line << " already exists";
			throw SyntaxError( __log );
		}
		http.servers[serverName] = __parseServer( http );
	}
	if ( http.servers.size() == 0) {
		__log << "config file must contain at least one server block";
		throw SyntaxError( __log );
	}
	__advance( Token::CLOSE_CURLY );
	__advance( Token::_EOF );
}


void	Parser::__advance( Token::token_t tok ) {
	if ( __currTok != tok ) {
		__log << "at line: " << __lexer.line;
		__log << " expected " << Token(tok) << " but found " <<  __currTok;
		throw SyntaxError( __log );
	}
	if ( __currTok != Token::_EOF )
		__currTok = __lexer.getNextToken();
}