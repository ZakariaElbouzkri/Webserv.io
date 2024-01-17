/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   servIO.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abizyane <abizyane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 21:00:57 by zel-bouz          #+#    #+#             */
/*   Updated: 2024/01/17 15:43:31 by abizyane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <poll.h>
#include <sys/select.h>
#include <utility>
#include <fstream>
#include <fcntl.h>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <map>
#include <set>
#include <vector>
#include <arpa/inet.h>

#define RESET "\033[0m"
#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define CONF_FILE_PATH "./configuration/servIO.conf"

typedef enum {
    HTTP_OK, HTTP_NOT_FOUND, HTTP_BAD_REQUEST, HTTP_INTERNAL_SERVER_ERROR, HTTP_NOT_IMPLEMENTED\
    , HTTP_SERVICE_UNAVAILABLE, HTTP_VERSION_NOT_SUPPORTED, HTTP_REQUEST_ENTITY_TOO_LARGE\
    , HTTP_REQUEST_URI_TOO_LONG, HTTP_UNSUPPORTED_MEDIA_TYPE, HTTP_METHOD_NOT_ALLOWED\
    , HTTP_REQUEST_HEADER_TOO_LARGE, HTTP_REQUEST_TIMEOUT, HTTP_BAD_GATEWAY, HTTP_UNPROCESSABLE_ENTITY\
    
} e_httpResponses;

extern std::map<std::string, std::string>   mimeTypes;
extern std::map<e_httpResponses, std::string> httpResponses;

#include "Token.hpp"
#include "Lexer.hpp"
#include "Poller.hpp"
#include "Socket.hpp"
#include "MainContext.hpp"
#include "Parser.hpp"

#include "../http/Request.hpp"
#include "../http/Response.hpp"

void	servCore( MainContext& main );
std::string&	strim( std::string str, const std::string& del = "\t\v\r\n\f ");
std::string		normPath( const std::string& path );
bool	ft_stoi( const std::string& str, int& result );

std::string getResponseError(e_httpResponses &response);