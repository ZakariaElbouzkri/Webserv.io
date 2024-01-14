/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poller.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-bouz <zel-bouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 12:26:56 by zel-bouz          #+#    #+#             */
/*   Updated: 2024/01/14 13:37:39 by zel-bouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "servIO.hpp"

Poller::Poller( void ) {
}


Poller::~Poller( void ) {
}

void	Poller::pushFd( int fd, short events ) {
	
struct pollfd	newFd;
	newFd.fd = fd;
	newFd.revents = 0;
	newFd.events = events;
	__pollfds.push_back(newFd);
}

struct FdEquals {
	int	targetFd;
	FdEquals(int fd) : targetFd(fd) {}
	bool operator()(const struct pollfd& pfd) const {
		return pfd.fd == targetFd;
	}
};

void Poller::erase(int fd) {
    
    __pollfds.erase(std::remove_if(__pollfds.begin(), __pollfds.end(),
		FdEquals(fd)), __pollfds.end());
}

int	Poller::poll( int timeout ) {
	if ( __pollfds.empty() )
		return -1;
	int	readySocket = ::poll( __pollfds.data(), __pollfds.size(), timeout );
	if ( readySocket < 0) {
		logs << ERROR << "servIO: " << "poll: " << strerror(errno) << END;
		return -1;
	}
	for ( size_t idx = 0; idx < __pollfds.size(); idx++ ) {
		if ( __pollfds[idx].revents & POLLIN || __pollfds[idx].revents & POLLOUT )
			return idx;
	}
	return -1;
}

struct pollfd& Poller::operator[]( const int& idx ) {
	return __pollfds.at(idx);
}
