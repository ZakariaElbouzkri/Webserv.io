/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poller.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-bouz <zel-bouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 12:26:59 by zel-bouz          #+#    #+#             */
/*   Updated: 2024/01/13 12:28:10 by zel-bouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "servIO.hpp"

class	Poller {
	public:
		Poller( void );
		~Poller( void );
		void	pushFd( int fd, short events = POLLIN );
		void	erase( int fd );
		int		poll( int timeout = 10 );
	
	private:
		std::vector<struct pollfd>	__pollfds;
};
