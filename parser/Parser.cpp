/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-bouz <zel-bouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 11:56:12 by zel-bouz          #+#    #+#             */
/*   Updated: 2023/12/27 20:26:13 by zel-bouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

Parser::Parser( void ) {
	__currTok = __lexer.getNextToken();
}

Parser::~Parser( void ) {
}

bool	checkStatusCode( std::string statusCode, int& ret ) {
	if (statusCode.size() != 3) return false;
	if (isdigit(statusCode[0]) && isdigit(statusCode[1]) && isdigit(statusCode[2])) {
		ret = atoi(statusCode.c_str());
		return (ret >= 100 && ret <= 599) ? true : false;
	}
	return false;
}

void	Parser::__parseErrorPage( HttpContext& httpCtx ) {
	__advance( Token::ERROR_PAGE );
	// TODO: implemet a function like stoi and checknumber if is valid;
	int code;
	if (checkStatusCode( __currTok.getData(), code ) == false) {
		__log << "invalid status code '" << __currTok.getData(); __logError();
	}
	__advance( Token::WORD );
	httpCtx.errorPages[code] = __currTok.getData();
	__advance( Token::WORD );
}

void	Parser::__parseLogs( HttpContext& httpCtx ) {
	if ( __currTok == Token::ACCESS_LOG ) {
		__advance( Token::ACCESS_LOG );
		httpCtx.logs.acces = __currTok.getData();
	} else	if ( __currTok == Token::ERROR_LOG ) {
		__advance( Token::ERROR_LOG );
		httpCtx.logs.err = __currTok.getData();
	} else  if ( __currTok == Token::ROOT ) {
		__advance( Token::ROOT );
		httpCtx.root = __currTok.getData();
	} else {
		__advance( Token::MAX_BODY );
		httpCtx.clientMaxBody = atoi( __currTok.getData().c_str() );
	}
	__advance( Token::WORD );
}

void	Parser::__parseMethods( HttpContext& httpCtx ) {
	__advance( Token::ALLOW );
	if ( __currTok != Token::WORD ) {
		__log << "exected allow methods found " << __currTok;
		__logError();
	}
	while ( __currTok == Token::WORD ) {
		httpCtx.allowedMethods.insert( __currTok.getData() );
		__advance( Token::WORD );
	}
}

void	Parser::__parseIndexes( HttpContext& httpCtx ) {
	if ( __currTok == Token::INDEX ) {
		__advance( Token::INDEX );
		if ( __currTok != Token::WORD )
			__advance( Token::WORD );
		while ( __currTok == Token::WORD ) {
			httpCtx.indexes.push_back( __currTok.getData() );
			__advance( Token::WORD );
		}
	} else {
		__advance( Token::AUTO_INDEX );
		if ( __currTok.getData() == "on" ) // may be check if data in ["on", "off"] and throw err;
			httpCtx.autoIndex = true;
		__advance( Token::WORD );
	}
}

void	Parser::__parseHttpDirectives( HttpContext& httpCtx ) {
	switch ( __currTok.getType() ) {
		case Token::ERROR_PAGE:	
			__parseErrorPage( httpCtx ); break;
		case Token::ACCESS_LOG:
		case Token::ROOT:
		case Token::MAX_BODY:
		case Token::ERROR_LOG:
			__parseLogs( httpCtx ); break;
		case Token::ALLOW:
			__parseMethods( httpCtx ); break;
		case Token::AUTO_INDEX:
		case Token::INDEX:
			__parseIndexes( httpCtx ); break;
		default:
			__log << "directive " << __currTok << " not in it's context";
			__logError();
	}
}


std::string	Parser::__parseKey( void ) {
	__advance( Token::OPEN_SQURE );

	std::string	key = __currTok.getData();

	__advance( Token::WORD );

	__advance( Token::CLOSE_SQUARE );
	return key;
}


ListenAddress	Parser::__parseListenAddr( void ) {
	ListenAddress	adr;
	__advance( Token::LISTEN );
	// TODO: implemet a function like stoi and checknumber if is valid;
	adr.port = atoi( __currTok.getData().c_str() );
	__advance( Token::WORD );
	return adr;
}


void	Parser::__parseRedirectPage( RedirectPage& page ) {
	__advance( Token::RETURN );
	if (checkStatusCode( __currTok.getData(), page.code ) == false) {
		__log << "invalid status code '" << __currTok.getData();
		__logError();
	}
	__advance( Token::WORD );
	page.path = __currTok.getData();
	__advance( Token::WORD );
}

LocationContext*	Parser::__parseLocation( HttpContext& httpCtx ) {
	__advance( Token::OPEN_CURLY );
	LocationContext* route = new LocationContext( httpCtx );

	while ( __currTok != Token::LOCATOIN && __currTok != Token::CLOSE_CURLY ) {
		
		if ( __currTok == Token::RETURN )
			__parseRedirectPage( route->page );
		else if ( __currTok == Token::CGI_ASSIGN ) {
			__advance( Token::CGI_ASSIGN );
			if ( __currTok != Token::WORD ) {
				__log << "expected cgi extentions, but found " << __currTok; __logError();
			}
			while ( __currTok == Token::WORD ) {
				__advance( Token::WORD );
				route->cgis.push_back( __currTok.getData() );
			}
		} else {
			__parseHttpDirectives( *route );
		}
		__advance( Token::SIMICOLEN );
	}
	
	while ( __currTok == Token::LOCATOIN ) {
		__advance( Token::LOCATOIN );
		std::string	locationPath = __parseKey();
		route->locations[locationPath] = __parseLocation( *route );
	}
	__advance( Token::CLOSE_CURLY );
	return route;
}

ServerContext*	Parser::__parseServer( HttpContext& httpCtx ) {
	__advance( Token::OPEN_CURLY );

	ServerContext* serv = new ServerContext( httpCtx );
	while ( __currTok != Token::LOCATOIN && __currTok != Token::CLOSE_CURLY ) {
		if ( __currTok == Token::LISTEN )
			serv->listen.push_back( __parseListenAddr( ) );
		else {
			__parseHttpDirectives( *serv );
		}
		__advance( Token::SIMICOLEN );
	}
	while ( __currTok == Token::LOCATOIN ) {
		__advance( Token::LOCATOIN );
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
		MainContext::servIter it = http.servers.find(serverName);
		if ( it != http.servers.end()) {
			__log << "duplicated servername: " << serverName;
			__logError();
		}
		http.servers[serverName] = __parseServer( http );
	}
	__advance( Token::CLOSE_CURLY );
	__advance( Token::END_OF_FILE );
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
	if ( __currTok != Token::END_OF_FILE )
		__currTok = __lexer.getNextToken();
}