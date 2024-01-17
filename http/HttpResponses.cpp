/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponses.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abizyane <abizyane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 17:00:40 by abizyane          #+#    #+#             */
/*   Updated: 2024/01/17 15:45:12 by abizyane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/servIO.hpp"

void    initHttpResponses(void){
    httpResponses[HTTP_OK] = "200 Ok";
    httpResponses[HTTP_NOT_FOUND] = "404 Not Found";
    httpResponses[HTTP_BAD_REQUEST] = "400 Bad Request";
    httpResponses[HTTP_INTERNAL_SERVER_ERROR] = "500 Internal Server Error";
    httpResponses[HTTP_SERVICE_UNAVAILABLE] = "503 Service Unavailable";
    httpResponses[HTTP_VERSION_NOT_SUPPORTED] = "505 HTTP Version Not Supported";
    httpResponses[HTTP_REQUEST_ENTITY_TOO_LARGE] = "413 Request Entity Too Large";
    httpResponses[HTTP_REQUEST_URI_TOO_LONG] = "414 Request-URI Too Long";
    httpResponses[HTTP_UNSUPPORTED_MEDIA_TYPE] = "415 Unsupported Media Type";
    httpResponses[HTTP_METHOD_NOT_ALLOWED] = "405 Method Not Allowed";
    httpResponses[HTTP_REQUEST_HEADER_TOO_LARGE] = "431 Request Header Fields Too Large";
    httpResponses[HTTP_REQUEST_TIMEOUT] = "408 Request Timeout";
    httpResponses[HTTP_BAD_GATEWAY] = "502 Bad Gateway";
    httpResponses[HTTP_UNPROCESSABLE_ENTITY] = "422 Unprocessable Entity";
    httpResponses[HTTP_NOT_IMPLEMENTED] = "501 Not Implemented";
}

std::string getResponseError(e_httpResponses &response){
    return httpResponses[response];
}