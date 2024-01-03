/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-bouz <zel-bouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/31 13:06:53 by zel-bouz          #+#    #+#             */
/*   Updated: 2024/01/03 18:38:52 by zel-bouz         ###   ########.fr       */
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

// i want to remove all the '/' duplicates and the last one if it exists
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
	return result;
}

std::string	joinPath( const std::string& path1, const std::string& path2 ) {
	std::string	result = normPath( path1 );
	if ( result.back() != '/' )
		result += '/';
	result += normPath( path2 );
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
