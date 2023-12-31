/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-bouz <zel-bouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 21:55:01 by zel-bouz          #+#    #+#             */
/*   Updated: 2023/12/30 05:50:00 by zel-bouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "servIO.hpp"

class	Lexer {
	public:
		size_t		line;
		Lexer( void );
		~Lexer( void );
		
		Token	getNextToken( void );
		bool	good( void ) const;
		
	private:
		std::string								__data;
		ssize_t									__pos;
		char									__curr;
		bool									__good;
		std::map<std::string, Token::token_t>	__keywords;
		
		void	__advance( void );
		Token	__parseWord( void );
		void	__skipComment( void );
};
