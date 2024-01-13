/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   servIO.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-bouz <zel-bouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 21:00:57 by zel-bouz          #+#    #+#             */
/*   Updated: 2024/01/13 15:37:06 by zel-bouz         ###   ########.fr       */
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

#include "Token.hpp"
#include "Lexer.hpp"
#include "Poller.hpp"
#include "Socket.hpp"
#include "MainContext.hpp"
#include "Parser.hpp"



void	servCore( MainContext& main );
std::string&	strim( std::string str, const std::string& del = "\t\v\r\n\f ");
std::string		normPath( const std::string& path );
bool	ft_stoi( const std::string& str, int& result );