/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poller.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-bouz <zel-bouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 12:26:56 by zel-bouz          #+#    #+#             */
/*   Updated: 2024/01/13 15:42:13 by zel-bouz         ###   ########.fr       */
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
	// else if (readySocket == 0) {
	// 	// Handle timeout (not possible here, but good practice)
	// 	logs << ERROR << "Timeout in poll" << END;
	// 	return -1;
	// }
	for ( size_t idx = 0; idx < __pollfds.size(); idx++ ) {
		if ( __pollfds[idx].revents & POLLIN || __pollfds[idx].revents & POLLOUT )
			return idx;
	}
	return -1;
}

	// int readySockets = poll(pollfds, 10 + 1, -1);

	// if (readySockets < 0) {
	// 	// Handle error
	// 	this->logs << ERROR << "Error in poll: " << strerror(errno) << END;
	// 	return -1;
	// }
	// else if (readySockets == 0) {
	// 	// Handle timeout (not possible here, but good practice)
	// 	this->logs << ERROR << "Timeout in poll" << END;
	// 	return -1;
	// }
	// int i = 0;
	// while (!(pollfds[i].revents & POLLIN) && !(pollfds[i].revents & POLLOUT))
	// 	i++;
	// return i;
