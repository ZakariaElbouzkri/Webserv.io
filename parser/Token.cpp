/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-bouz <zel-bouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 21:42:12 by zel-bouz          #+#    #+#             */
/*   Updated: 2024/01/02 20:49:17 by zel-bouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "servIO.hpp"

Token::Token( const std::string& data ) : __type(WORD), __data(data) {
}

Token::Token( token_t type ) : __type( type ) {
}

bool	Token::operator==( token_t rhs ) {
	return rhs == __type;
}

bool	Token::operator!=( token_t rhs ) {
	return rhs != __type;
}

bool	Token::isKey( void ) const {
	return __type >= HTTP && __type <= CGI;
}

Token::token_t	Token::getType( void ) const {
	return __type;
}

const std::string&	Token::getData( void ) const {
	return __data;
}

std::ostream&	operator<<( std::ostream& os, const Token& rhs ) {
	std::string	keys[21] = {
		"value " + rhs.__data  , "{", "}", "[", "]", ";", "http", "server", "location",
		"error_page", "error_log", "access_log", "allow", "client_max_body_size",
		"index", "autoindex", "redirect", "listen", "root", "cgi", "_EOF",
	};
	os << keys[(int)rhs.__type] << " " << rhs.__data << (rhs.isKey() ? "keyword" : "" );
	return os;
}
