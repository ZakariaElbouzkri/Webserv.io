/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   servIO.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-bouz <zel-bouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 12:40:18 by zel-bouz          #+#    #+#             */
/*   Updated: 2023/12/27 20:54:49 by zel-bouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "servIO.hpp"

int main() {
	MainContext	main;
	try {
		Parser().parse( main );
	} catch (std::exception &e) {
		std::cout << e.what() << '\n';
	}
}
