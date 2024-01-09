/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   servIO.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-bouz <zel-bouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 12:40:18 by zel-bouz          #+#    #+#             */
/*   Updated: 2023/12/31 16:09:48 by zel-bouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "servIO.hpp"

LogStream	logs;
ErrorPage	erPages;

#define ERROR404 "<h1> 404 - Not found </h1>"

/*
	sdsd / HTTP/1.1
	.....
*/

int main() {
	MainContext main( logs, erPages );
	try {
		Parser		parser;
		parser.parse( main );
	} catch( std::exception& e) {
		std::cerr << e.what() << '\n';
		return ( EXIT_FAILURE );
	}
}
