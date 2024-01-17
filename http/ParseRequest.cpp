/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseRequest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abizyane <abizyane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 13:46:10 by abizyane          #+#    #+#             */
/*   Updated: 2024/01/17 20:53:09 by abizyane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/servIO.hpp"



static std::string  trimStr(std::string& line){	
	std::string   str(line);

	str.erase(0, str.find_first_not_of(" \t\n\r\f\v"));
	str.erase(str.find_last_not_of(" \t\n\r\f\v") + 1);
	return str;
}

static size_t	countDelimiters(std::string& str, std::string& delim){
	size_t i = 0;

	for (size_t j = 0; j < str.size(); j++)
		if (str.substr(j, delim.size()) == delim)
			i++;
	return (i);
}

std::vector<std::string> ft_strtok(std::string str, std::string delims) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, '\n')) {
        size_t pos;
        while ((pos = token.find_first_of(delims)) != std::string::npos) {
            if (pos > 0) {
                tokens.push_back(token.substr(0, pos));
            }
            token = token.substr(pos + 1);
        }
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    return tokens;
}

bool	Request::parseRequestLine(std::string& requestLine){
	std::stringstream	ss(requestLine);
	std::string str;
	std::getline(ss, str, '\n');
	std::vector<std::string> line = ft_strtok(trimStr(str), " ");

	if (line.size() != 3 || !_context.servers[_serverName]->has(line[0]) || line[2] != "HTTP/1.1"){
		if (!_context.servers[_serverName]->has(line[0]))
			_statusCode = HTTP_NOT_IMPLEMENTED;
		else if (line[2] != "HTTP/1.1")
			_statusCode = HTTP_VERSION_NOT_SUPPORTED;
		else if (line[1].size() > 2083)
			_statusCode = HTTP_REQUEST_URI_TOO_LONG;
		else
			_statusCode = HTTP_BAD_REQUEST;
		_state = Error;
		return false;
	}
	_requestMethod = line[0];
	_requestUri = line[1];
	_state = Headers;
	requestLine.erase(0, str.size());
	return true;
}


bool	Request::parseHeaders(std::pair<std::string, std::string>& token){

	// if (headers["Host"] == "")
	// 	throw httpException();
	// if (headers["Transfer-Encoding"] == "chunked")
	// 	_chunked = true;
	// if (headers["Content-Length"] != "")
	// 	_contentLength = std::stoi(headers["Content-Length"]);
	// if (headers["Content-Type"] != "")
	// 	_contentType = headers["Content-Type"];
	// return true;
		
}

bool	Request::addHeader(std::string& line){
	std::stringstream	ss(line);
	std::string str;
	std::getline(ss, str, '\n');
	std::string	trimmed = trimStr(str);

	std::pair<std::string, std::string> token;
	token.first = trimmed.substr(0 ,trimmed.find_first_of('='));
	token.second = trimmed.substr(token.first.size() + 1, trimmed.size() - (token.first.size() + 1));

	if (token.second == "" || _headers.find(token.first) != _headers.end() || !parseHeaders(token))
		ft_error(HTTP_BAD_REQUEST);

	_headers[token.first] = token.second;
	// if (line.substr(line.size() - 2, 2) == "\r\n" && line.substr(line.size() - 4, 2) == "\r\n")
	line.erase(0, str.size());
	std::getline(ss, str);
	if (str == "\r")
		_state = Body;
	return true;
}

void        Request::parseRequest(std::string& request){
	
	here:
	getLineState();
	if (_state < Ready && _lineState != Chunk){
		switch (_state){
			case RequestLine:
				if (!parseRequestLine(_currentLine))
					ft_error(HTTP_BAD_REQUEST);
				if (_state != Headers)
					break;
			case Headers:
				if (!addHeader(_currentLine))
					ft_error(HTTP_BAD_REQUEST);
				if (_state != Body || _requestMethod == "GET"){
					if (_state == Body && _requestMethod == "GET")
						_state = Ready;
					break;
				}
			case Body:
				if (!parseBody(_currentLine))
					ft_error(HTTP_BAD_REQUEST);
				break;
			default:
				_currentLine = request;
				_state = RequestLine;
				goto here;
		}
	}
	else if (_state < Ready){
		_currentLine += request;
		goto here;
	}
	_response = prepareResponse();
}
		