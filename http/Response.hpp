/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abizyane <abizyane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 23:07:10 by abizyane          #+#    #+#             */
/*   Updated: 2024/01/17 15:51:28 by abizyane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once

#include "../includes/servIO.hpp"

typedef enum {
		Ok = 200,
		Not_Found = 404,


} e_status;

class Response {
	private :
		std::string							_response;
		bool								_good;
		e_status							_statusCode;
		int									_contentLen;
		int									_maxLen;
		std::string							_contentType;
		std::map<std::string, std::string>  _headers;
		std::string 						_body;
		

	public :
		Response(int status);
		Response(const Request& request, MainContext& context);
		~Response();

		bool	good(void);
		
		std::string& GetResponse(void);
		std::string& GetHeader(const std::string& key);
		int			 GetLength(void);
		std::string& GetBody(void);
		std::string& GetContentType(void);
		e_status	 GetStatusCode(void);
		std::map<std::string, std::string>& GetHeaders(void);
	
		void	fillHeaders(std::map<std::string, std::string>& headers);
		void	fillBody(std::string& body);
		void	fillStatusCode(e_status code);
		void	fillContentType(std::string& type);
		void	fillLength(size_t length);
		
		

};

//Example :
		// HTTP/1.1 200 OK
		// Date: Mon, 27 Jul 2009 12:28:53 GMT
		// Server: Apache/2.2.14 (Win32)
		// Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT
		// Content-Length: 88
		// Content-Type: text/html
		// Connection: Closed