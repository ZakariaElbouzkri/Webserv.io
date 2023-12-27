/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-bouz <zel-bouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 21:21:47 by zel-bouz          #+#    #+#             */
/*   Updated: 2023/12/27 21:01:19 by zel-bouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "servIO.hpp"

class	Token {
	public:
		typedef enum {
			OPEN_CURLY = 0, CLOSE_CURLY, OPEN_SQURE, CLOSE_SQUARE, SIMICOLEN, WORD, END_OF_FILE,
			/* keywords */ 
			HTTP, SERVER, LOCATOIN, ROOT, SERVER_NAME, ERROR_PAGE, ACCESS_LOG, 
			ERROR_LOG, ALLOW, LISTEN, INDEX, AUTO_INDEX, RETURN, MAX_BODY, 
			CGI_ASSIGN
		}	token_t;
		Token( void );
		Token( token_t tok );
		Token( std::string data );
		Token( const char *data );
		
		const std::string&	getData( void ) const;
		token_t				getType( void ) const;
		bool				isKey( void ) const;
		bool				operator==( token_t type ) const;
		// bool				operator==( int type ) const;
		bool				operator!=( token_t type ) const;

		friend std::ostream&	operator<<( std::ostream& os, const Token& rhs );
		
	private:
		token_t		__type;
		std::string	__data;
};
