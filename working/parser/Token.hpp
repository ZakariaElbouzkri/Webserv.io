/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-bouz <zel-bouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 21:32:51 by zel-bouz          #+#    #+#             */
/*   Updated: 2023/12/30 05:50:17 by zel-bouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "servIO.hpp"

class	Token {
	public:
		typedef enum {
			WORD, OPEN_CURLY, CLOSE_CURLY, OPEN_SQUARE, CLOSE_SQUARE, SIMICOLEN,
			HTTP, SERVER, LOCATION, ERR_PAGE, ERR_LOG, ACC_LOG, ALLOW, MAX_BODY,
			INDEX, AUTOINDEX, RETURN, LISTEN, ROOT, CGI, _EOF
		} token_t;

		Token( const std::string& data);
		Token( token_t type );
		bool	operator==( token_t rhs );
		bool	operator!=( token_t rhs );
		bool	isKey( void ) const;
		token_t	getType( void ) const;
		const std::string&	getData( void ) const;
		
		friend	std::ostream&	operator<<( std::ostream& os, const Token& rhs );

	private:
		token_t		__type;
		std::string	__data;
};

