/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-bouz <zel-bouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 15:14:06 by zel-bouz          #+#    #+#             */
/*   Updated: 2023/12/27 20:55:41 by zel-bouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once


#include "servIO.hpp"

class	Lexer {
	public:
		ssize_t	line;

		Lexer( void );
		~Lexer( void );
		Token	getNextToken( void );
		bool	good( void ) const ;
	private:
		bool									__good;
		ssize_t									__pos;
		char									__curr;
		std::string								__data;
		std::map<std::string, Token::token_t>	__keywords;

		typedef std::map<std::string, Token::token_t>::iterator Iter;
	
		void	__init_keys( void );
		void	__advance( void );
		void	__skip_spaces( void );
		void	__skip_commet( void );
		Token	__parseWord( void );
};


