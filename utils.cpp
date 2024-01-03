/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-bouz <zel-bouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/31 13:06:53 by zel-bouz          #+#    #+#             */
/*   Updated: 2024/01/03 22:38:23 by zel-bouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "servIO.hpp"

std::string&	strim( std::string str, const std::string& del ) {
	int start = 0;
	while ( strchr( del.c_str(), str[start] ) )
		start++;
	int	end = str.size() - 1;
	while ( strrchr( del.c_str(), str[end] ) )
		end--;
	if ( end - start + 1 <= 0)
		return ( str = "" );
	return  str = str.substr( start, end - start + 1 );		
}

std::string	normPath( const std::string& path ) {
	std::string	result;

	for ( size_t i = 0; i < path.size(); i++ ) {
		if ( path[i] == '/' && ( i == 0 || path[i - 1] != '/' ) )
			result += path[i];
		else if ( path[i] != '/' )
			result += path[i];
	}
	if ( result.size() > 1 && result.back() == '/' )
		result.pop_back();
	if ( result[0] != '/' )
		result = '/' + result;
	return result;
}

bool	ft_stoi( const std::string& str, int& result ) {
	std::stringstream	ss;
	ss << str;
	ss >> result;
	if ( ss.eof() != true )
		return false;
	return true;
}
