/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-bouz <zel-bouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/31 13:06:53 by zel-bouz          #+#    #+#             */
/*   Updated: 2023/12/31 13:26:35 by zel-bouz         ###   ########.fr       */
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
