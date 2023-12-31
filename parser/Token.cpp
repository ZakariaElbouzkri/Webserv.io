/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-bouz <zel-bouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 21:42:12 by zel-bouz          #+#    #+#             */
/*   Updated: 2023/12/30 05:50:15 by zel-bouz         ###   ########.fr       */
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
	return __type >= HTTP && __type <= ROOT;
}

Token::token_t	Token::getType( void ) const {
	return __type;
}

const std::string&	Token::getData( void ) const {
	return __data;
}

std::ostream&	operator<<( std::ostream& os, const Token& rhs ) {
	std::string	keys[21] = {
		"WORD", "OPEN_CURLY", "CLOSE_CURLY", "OPEN_SQUARE", "CLOSE_SQUARE", "SIMICOLEN",
		"HTTP", "SERVER", "LOCATION", "ERR_PAGE", "ERR_LOG", "ACC_LOG", "ALLOW", "MAX_BODY",
		"INDEX", "AUTOINDEX", "RETURN", "LISTEN", "ROOT", "CGI", "_EOF",
	};
	os << keys[(int)rhs.__type] << " " << rhs.__data;
	return os;
}
