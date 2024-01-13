/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poller.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-bouz <zel-bouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 12:26:56 by zel-bouz          #+#    #+#             */
/*   Updated: 2024/01/13 12:28:07 by zel-bouz         ###   ########.fr       */
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

void Poller::erase(int fd) {
    struct FdEquals {
        int	targetFd;
        FdEquals(int fd) : targetFd(fd) {}
        bool operator()(const struct pollfd& pfd) const {
            return pfd.fd == targetFd;
        }
    };
    __pollfds.erase(std::remove_if(__pollfds.begin(), __pollfds.end(),
		FdEquals(fd)), __pollfds.end());
}

int	Poller::poll( int timeout ) {
	int	ret = ::poll( __pollfds.data(), __pollfds.size(), timeout );
	if (ret == -1) {
		// handle poll error;
	}
	return ret;
}
