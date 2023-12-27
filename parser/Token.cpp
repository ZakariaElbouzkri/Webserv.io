/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-bouz <zel-bouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 21:21:30 by zel-bouz          #+#    #+#             */
/*   Updated: 2023/12/27 18:13:57 by zel-bouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Token.hpp"

Token::Token( void ) : __type( Token::END_OF_FILE ) {
}

Token::Token( token_t tok ) : __type( tok ) {
}

Token::Token( std::string data ) : __type( Token::WORD ), __data( data ) {
}

Token::Token( const char *data ) : __type(Token::WORD ), __data( data ) {
}

const std::string&	Token::getData( void ) const {
	return __data;
}

bool	Token::isKey( void ) const {
	return (this->__type >= HTTP && this->__type <= CGI_ASSIGN);
}

bool	Token::operator!=( token_t tok ) const {
	return (this->__type != tok);
}

bool	Token::operator==( token_t tok ) const {
	return (this->__type == tok);
}

Token::token_t	Token::getType( void ) const {
	return (this->__type);
}

// bool	Token::operator==( int type ) const {
// 	if (type > 21 || *this != (token_t)type);
// 		return false;
// 	return true;
// }

std::ostream&	operator<<( std::ostream& os, const Token& rhs ) {
	std::string types[] = {
		"{", "}", "[", "]", ";", "value", "EOF", "http",
		"server", "location", "root", "server_name", "error_page", "acces_log",
		"error_log", "allow", "listen", "index", "autoindex", "return", "max_body", "cgi_assign",
	};
	os << "'" << RED << types[( int ) rhs.__type];
	if (rhs.isKey())
		os << " keyword";
	os  << RESET << "'";
	return (os);
}
