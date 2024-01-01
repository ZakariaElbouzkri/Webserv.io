/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   servIO.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakebli <nakebli@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 12:40:18 by zel-bouz          #+#    #+#             */
/*   Updated: 2024/01/01 17:54:37 by nakebli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "servIO.hpp"
#include "ServerContext.hpp"

LogStream	logs;
ErrorPage	erPages;

#define ERROR404 "<h1> 404 - Not found </h1>"

int main() {
	MainContext main(logs, erPages);
	try {
		Parser parser;
		parser.parse(main);
	} catch (std::exception &e) {
		std::cerr << e.what() << '\n';
		return (EXIT_FAILURE);
	}
	for (std::map<std::string, ServerContext *>::iterator it = main.servers.begin(); it != main.servers.end(); it++) {
		// it->second->();
		std::cout << it->first << "   " << it->second->serverSocket.getFd() << std::endl;
	}
}
