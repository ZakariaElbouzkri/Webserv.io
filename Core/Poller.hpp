/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poller.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-bouz <zel-bouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 12:26:59 by zel-bouz          #+#    #+#             */
/*   Updated: 2024/01/13 15:23:36 by zel-bouz         ###   ########.fr       */
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
		// void	pushFd( vector<int> fds, events = POLLIN );
		void	erase( int fd );
		int		poll( int timeout = 10 );
		struct pollfd& operator[]( const int& idx ) {
			return __pollfds.at(idx);
		};
	
	private:
		std::vector<struct pollfd>	__pollfds;
};
