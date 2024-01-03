/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-bouz <zel-bouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 21:34:21 by zel-bouz          #+#    #+#             */
/*   Updated: 2024/01/03 19:06:25 by zel-bouz         ###   ########.fr       */
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
		void				__parseMethods( HttpMethods& methods );
		void				__parseIndexes( HttpContext& httpCtx );
		void				__parseCgiExt( std::vector<std::string>& cgi );
		void				__parseRedirectPage( std::pair<int, std::string>& page );
		std::string			__parseKey( void );
		ServerContext*		__parseServer( HttpContext& httpCtx );
		LocationContext*	__parseLocation( HttpContext& httpCtx );
		ListenAddress		__parseListenAddr( void );
		// void				__logError( bool logLine = true );
		
	public:
		class	SyntaxError : public std::exception {
			std::string	__msg;
			public:
				SyntaxError( std::stringstream& log ) throw() {
					__msg = "webserv: config: " + log.str();
				}
				const char * what( void ) const _NOEXCEPT {
					return __msg.c_str();
				}
				virtual ~SyntaxError() _NOEXCEPT {};
		};
		class	FileNotFoundOrEmpty : public std::exception {
			public:
				const char * what( void ) const _NOEXCEPT {
					return "webserv: config: file not found or empty";
				}
		};
		void	parse( MainContext& http );
		
		Parser( void );
		~Parser( void );
};

// global variables 
extern	LogStream	logs;
extern	ErrorPage	erPages;
