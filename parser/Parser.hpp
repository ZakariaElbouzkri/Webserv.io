/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-bouz <zel-bouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 08:53:29 by zel-bouz          #+#    #+#             */
/*   Updated: 2023/12/27 20:55:15 by zel-bouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "servIO.hpp"

class	Parser {
	private:
		Lexer				__lexer;
		Token				__currTok;
		std::stringstream	__log;

		void				__advance( Token::token_t tok );
		void				__parseHttpDirectives( HttpContext& httpCtx );
		void				__parseErrorPage( HttpContext& httpCtx );
		void				__parseLogs( HttpContext& httpCtx );
		void				__parseMethods( HttpContext& httpCtx );
		void				__parseIndexes( HttpContext& httpCtx );
		void				__parseRedirectPage( RedirectPage& page );
		std::string			__parseKey( void );
		ServerContext*		__parseServer( HttpContext& httpCtx );
		LocationContext*	__parseLocation( HttpContext& httpCtx );
		ListenAddress		__parseListenAddr( void );
		void				__logError( void );
		
	public:
		class	SyntaxError : public std::exception {
			public:
				const char * what( void ) const _NOEXCEPT {
					return "";
				}
		};
		void	parse( MainContext& http );
		
		Parser( void );
		~Parser( void );
			
};

