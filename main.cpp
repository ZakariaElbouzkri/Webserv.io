/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abizyane <abizyane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 12:40:18 by zel-bouz          #+#    #+#             */
/*   Updated: 2024/01/14 14:38:29 by abizyane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "servIO.hpp"

LogStream	logs;

std::map<std::string, std::string> mimeTypes;


int	main() {
	MainContext	http( logs );
	try {
		Parser	parser;
		parser.parse( http );
	} catch( std::exception & e ) {
		std::cerr << e.what() << std::endl;
	}
	init_mimeTypes();
	servCore( http );
}
