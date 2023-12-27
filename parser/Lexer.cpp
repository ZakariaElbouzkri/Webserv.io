/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-bouz <zel-bouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 15:19:33 by zel-bouz          #+#    #+#             */
/*   Updated: 2023/12/27 21:06:22 by zel-bouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "servIO.hpp"

Lexer::Lexer( void ) : __good(false), __pos(1) {
	this->line = 0;
	std::ifstream	_conf( CONF_FILE_PATH );
	__good = false;
	if ( _conf.is_open() == false || !std::getline( _conf, __data, '\0' ) ) {
		_conf.close();
		return ;
	}
	__curr = __data[__pos];
	__good = true;
	__init_keys();
}

void	Lexer::__init_keys( void ) {
	std::string		keys[15] = { "http", "server", "location", "root", "server_name", "error_page", 
		"access_log", "error_log", "allow", "listen", "index", "autoindex", "return", "client_max_body_size", "cgi_assign" };
	Token::token_t	values[15] = { Token::HTTP, Token::SERVER, Token::LOCATOIN, Token::ROOT, Token::SERVER_NAME, Token::ERROR_PAGE, Token::ACCESS_LOG,
		Token::ERROR_LOG, Token::ALLOW, Token::LISTEN, Token::INDEX, Token::AUTO_INDEX, Token::RETURN, Token::MAX_BODY, 
		Token::CGI_ASSIGN };
	for ( int itr = 0; itr < 15; itr++ ) {
		__keywords[keys[itr]] = values[itr];
	}
}

Lexer::~Lexer( void ) {
}

Token	Lexer::getNextToken( void ) {
	while ( __curr != '\0' ) {
		if ( isspace( __curr ) ) {
			__skip_spaces();
		} else {
			switch ( __curr ) {
				case '#':	__skip_commet();	break;
				case '{':	__advance();		return Token::OPEN_CURLY;
				case '}':	__advance();		return Token::CLOSE_CURLY;
				case '[':	__advance();		return Token::OPEN_SQURE;
				case ']':	__advance();		return Token::CLOSE_SQUARE;
				case ';':	__advance();		return Token::SIMICOLEN;
				default:						return __parseWord();
			}
		}
	}
	return Token::END_OF_FILE;
}

Token	Lexer::__parseWord( void ) {
	std::string	result;

	while ( __curr && !strchr( "[]{};#\t\r\f\v\n ", __curr ) ) {
		result += __curr;
		__advance();
	}
	Iter	it = __keywords.find( result );
	if ( it != __keywords.end() ) 
		return it->second;
	return result;
}

void	Lexer::__advance( void ) {
	if ( __curr == '\0' )
			return ;
	__curr = __data[++__pos];
}

void	Lexer::__skip_commet( void ) {
	while ( __curr && __curr != '\n' )
		__advance();
	this->line += ( __curr == '\n');
	__advance();
}

void	Lexer::__skip_spaces( void ) {
	while ( __curr && isspace( __curr ) ) {
		this->line += ( __curr == '\n');
		__advance();
	}
}

bool	Lexer::good( void ) const {
	return __good;
}