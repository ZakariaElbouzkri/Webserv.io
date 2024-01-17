/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeTypes.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abizyane <abizyane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 17:01:06 by abizyane          #+#    #+#             */
/*   Updated: 2024/01/17 15:45:47 by abizyane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/servIO.hpp"

void	initMimeTypes( void ) {
    mimeTypes["html"]= "text/html";
    mimeTypes["shtml"]= "text/html";
    mimeTypes["htm"]= "text/html";
    mimeTypes["css"]= "text/css";
    mimeTypes["xml"]= "text/xml";
    mimeTypes["gif"]= "image/gif";
    mimeTypes["jpeg"]= "image/jpeg";
    mimeTypes["jpg"]= "image/jpeg";
    mimeTypes["js"]= "application/javascript";
    mimeTypes["txt"]= "text/plain";
    mimeTypes["png"]= "image/png";
    mimeTypes["svg"]= "image/svg+xml";
    mimeTypes["svgz"]= "image/svg+xml";
    mimeTypes["tif"]= "image/tiff";
    mimeTypes["tiff"]= "image/tiff";
    mimeTypes["wbmp"]= "image/vnd.wap.wbmp";
    mimeTypes["webp"]= "image/webp";
    mimeTypes["ico"]= "image/x-icon";
    mimeTypes["jng"]= "image/x-jng";
    mimeTypes["bmp"]= "image/x-ms-bmp";
    mimeTypes["woff"]= "font/woff";
    mimeTypes["woff2"]= "font/woff2";
    mimeTypes["json"]= "application/json";
    mimeTypes["pdf"]= "application/pdf";
    mimeTypes["wasm"]= "application/wasm";
    mimeTypes["xspf"]= "application/xspf+xml";
    mimeTypes["zip"]= "application/zip";
    mimeTypes["dmg"]= "application/octet-stream";
    mimeTypes["iso"]= "application/octet-stream";
    mimeTypes["img"]= "application/octet-stream";
    mimeTypes["mp3"]= "audio/mpeg";
    mimeTypes["ogg"]= "audio/ogg";
    mimeTypes["m4a"]= "audio/x-m4a";
    mimeTypes["mp4"]= "video/mp4";
    mimeTypes["mpeg;"]= "video/mpeg";
    mimeTypes["mpg"]= "video/mpeg";
    mimeTypes["mov"]= "video/quicktime";
    mimeTypes["webm"]= "video/webm";
} 