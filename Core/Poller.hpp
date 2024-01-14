/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poller.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-bouz <zel-bouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 12:26:59 by zel-bouz          #+#    #+#             */
/*   Updated: 2024/01/14 13:36:57 by zel-bouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "servIO.hpp"

class	Poller {
	public:
	
		typedef std::vector<struct pollfd>::iterator PollIter;

		Poller( void );
		~Poller( void );
		void	pushFd( int fd, short events = POLLIN );
		void	erase( int fd );
		int		poll( int timeout = 10 );
		struct pollfd& operator[]( const int& idx );
	private:
		std::vector<struct pollfd>	__pollfds;
};
