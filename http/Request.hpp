/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abizyane <abizyane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 23:31:26 by abizyane          #+#    #+#             */
/*   Updated: 2024/01/17 20:37:58 by abizyane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../includes/servIO.hpp"

typedef enum{ Waiting, RequestLine, Headers, Body, Ready, Done, Error} e_state;
typedef	enum{ Empty, Chunk, Line, Complete} e_lineState;

class Request {
	private :
		std::string							_currentLine;
		std::string							_serverName;
		std::string 						_requestMethod;
		std::string 						_requestUri;
		e_httpResponses						_statusCode;
		e_state								_state;
		e_lineState							_lineState;
		bool								_isChunked;
		std::map<std::string, std::string>  _headers;
		std::string 						_body;
		MainContext&						_context;
		Response*							_response;

	public:
		Request(MainContext &context);
		~Request();

		void    		parseRequest(std::string& request);
		bool			parseRequestLine(std::string& line);
		bool			parseHeaders(std::pair<std::string, std::string>& token);
		bool			parseHeader(std::string& line);
		bool			addHeader(std::string& line);
		bool			parseBody(std::string& line);
		

		Response*   	prepareResponse(void);
		void			getLineState(void);
		bool			getState(void) const;
		void			ft_error(e_httpResponses code);


		const std::string&	getUri(void) const;
		// std::string&	getType(const std::string& key) const;
		const std::string&	getMethod(void) const;
		const std::string&	getBody(void) const;
		Response*		getResponse(void);
		const std::map<std::string, std::string>&	GetHeaders(void) const;

};

//Examples: 
	//=> Get :
		// GET /hello.htm HTTP/1.1
		// User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)
		// Host: www.tutorialspoint.com
		// Accept-Language: en-us
		// Accept-Encoding: gzip, deflate
		// Connection: Keep-Alive
//~~~~~~~~~~~~~~~~~~
		// Received: GET / HTTP/1.1
		// Host: localhost:8001
		// Connection: keep-alive
		// sec-ch-ua: "Not_A Brand";v="8", "Chromium";v="120", "Google Chrome";v="120"
		// sec-ch-ua-mobile: ?0
		// sec-ch-ua-platform: "macOS"
		// Upgrade-Insecure-Requests: 1
		// User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36
		// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7
		// Sec-Fetch-Site: none
		// Sec-Fetch-Mode: navigate
		// Sec-Fetch-User: ?1
		// Sec-Fetch-Dest: document
		// Accept-Encoding: gzip, deflate, br
		// Accept-Language: en-US,en;q=0.9,ar;q=0.8

//----------------------------------------------------------
	//=> Post :
		// POST /cgi-bin/process.cgi HTTP/1.1
		// User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)
		// Host: www.tutorialspoint.com
		// Content-Type: application/x-www-form-urlencoded
		// Content-Length: length
		// Accept-Language: en-us
		// Accept-Encoding: gzip, deflate
		// Connection: Keep-Alive
		
		// licenseID=string&content=string&/paramsXML=string
//----------------------------------------------------------
