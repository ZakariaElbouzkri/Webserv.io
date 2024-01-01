/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   servIO.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakebli <nakebli@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 21:00:57 by zel-bouz          #+#    #+#             */
/*   Updated: 2024/01/01 17:37:48 by nakebli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <poll.h>
#include <fcntl.h>
#include <sys/select.h>
#include <utility>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
#include <set>
#include <vector>

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
#include "MainContext.hpp"
#include "Parser.hpp"




std::string&	strim( std::string str, const std::string& del = "\t\v\r\n\f ");