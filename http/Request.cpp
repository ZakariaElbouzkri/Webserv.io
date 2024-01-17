/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abizyane <abizyane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 23:30:13 by abizyane          #+#    #+#             */
/*   Updated: 2024/01/17 21:01:27 by abizyane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(MainContext &context): _context(context){
    _state = Waiting;
    _lineState = Empty;
    _isChunked = false;
    _headers.clear();
    _response = NULL;
    _statusCode = HTTP_UNPROCESSABLE_ENTITY;

}

Response *Request::prepareResponse(void){
    if (_state == Ready){
        Response *response = new Response(*this, _context);
        response->fillHeaders(_headers);
        response->fillBody(_body);
        
 
        return response;   
    }
    return NULL;
}

Response*   Request::getResponse(void){
    return (_response);   
}

// std::string&    Request::getType(const std::string& key) const{
// 	return mimeTypes[key] ;
// }

void  Request::getLineState(void){
    for(size_t i = 0; i + 2 < _currentLine.size(); i++)
        if (_currentLine.substr(i, i + 2) == "\r\n"){
            if (i + 2 == _currentLine.size() - 1)
                _lineState = Line;
            if (_currentLine.substr(i, i + 2) == "\r\n")
                _lineState = Complete;
            // else if (_currentLine.substr(i, i + 2) != "\r\n")
            //     _lineState = Save;
            return;
        }
    _lineState = Chunk;
    if (_currentLine == "")
        _lineState = Empty;
}

void        Request::ft_error(e_httpResponses code){
    _statusCode = code;
    _state = Error;
}

bool        Request::getState(void) const{
    return (_state == Done);
}

const std::string&	Request::getUri(void) const{
        return (_requestUri);
}
    
const std::string&	Request::getMethod(void) const{
    return (_requestMethod);
}
    
const std::string&	Request::getBody(void) const{
    return (_body);
        
}
    
const std::map<std::string, std::string>&	Request::GetHeaders(void) const{
    return (_headers);
}  

Request::~Request(){
    _headers.clear();
    if (_response)
        delete _response;
}
