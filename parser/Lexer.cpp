/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-bouz <zel-bouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 21:58:33 by zel-bouz          #+#    #+#             */
/*   Updated: 2024/01/02 21:06:20 by zel-bouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "servIO.hpp"

Lexer::Lexer( void ) : line(1), __good(false) {
	std::ifstream	conf( CONF_FILE_PATH );
	if ( !conf.is_open() || !std::getline( conf, __data, '\0' ) )
		return ;
	__curr = __data[ ( __pos = 0 ) ];
	__good = true;
	std::string	keys[14] = {
		"http", "server", "location", "error_page", "error_log", "access_log", "allow", 
		"client_max_body_size", "index", "autoindex", "redirect", "listen", "root", "cgi_assign"
	};
	Token::token_t	vals[14] = {
		Token::HTTP, Token::SERVER, Token::LOCATION, Token::ERR_PAGE, Token::ERR_LOG, 
		Token::ACC_LOG, Token::ALLOW, Token::MAX_BODY,
		Token::INDEX, Token::AUTOINDEX, Token::RETURN, Token::LISTEN, Token::ROOT, Token::CGI
	};
	for ( int i = 0; i < 14; i++ ) {
		__keywords[ keys[i] ] = vals[i]; 
	}
	conf.close();
}

Lexer::~Lexer( void ) {
}

void	Lexer::__advance( void ) {
	this->line +=  __curr == '\n' ;
	if ( __curr != '\0' )
		__curr = __data[ ++__pos ];
}

void	Lexer::__skipComment( void ) {
	while ( __curr != '\0' && __curr != '\n' )
		__advance( );
	__advance( );
}


Token	Lexer::__parseWord( ) {
	std::string	result;
	if ( __curr == '\"' ) {
		__advance();
		while ( __curr != '\0' && __curr != '\"') {
			result += __curr;
			__advance();
		}
		if ( __curr != '\"' ) return Token::_EOF;
		__advance();
		result = strim( result );
		if ( result.empty() ) return getNextToken();
		return result;
	}
	while ( __curr != '\0' && !strchr( "#{}[];\t\v\r\n ", __curr ) ) {
		result += __curr;
		__advance();
	}
	std::map<std::string, Token::token_t>::iterator it = __keywords.find(result);
	if ( it != __keywords.end() )
		return it->second;
	return result;
}



Token	Lexer::getNextToken( void ) {
	while ( __curr != '\0' ) {
		if ( isspace( __curr ))
			__advance( );
		else {
			switch ( __curr ) {
				case '{':	__advance( ); return Token::OPEN_CURLY;
				case '}':	__advance( ); return Token::CLOSE_CURLY;
				case '[':	__advance( ); return Token::OPEN_SQUARE;
				case ']':	__advance( ); return Token::CLOSE_SQUARE;
				case ';':	__advance( ); return Token::SIMICOLEN;
				case '#':	__skipComment( ); break;
				default:	return __parseWord(  );
			}	
		}
	}
	return Token::_EOF;
}

bool	Lexer::good( void ) const {
	return __good;
}
