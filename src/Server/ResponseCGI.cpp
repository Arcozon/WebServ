/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseCGI.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 16:22:44 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/19 18:17:50 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "CGI.hpp"

static void	_getContextCGI(const std::string &URI,
				std::string &scriptName,
				std::string &pathInfo)
{
	std::string::size_type	startScriptName = URI.find_first_not_of('/'); 
	if (startScriptName == std::string::npos)
		startScriptName = 0;
	std::string::size_type	endScriptName = URI.find_first_of("/", startScriptName);
	
	scriptName = URI.substr(startScriptName, endScriptName - startScriptName);
	
	if (endScriptName != std::string::npos)
		pathInfo = URI.substr(endScriptName);
}

void	Response::_handleCGI(void)
{
	std::string scriptName, pathInfo, queryString;

	_getContextCGI(_URI, scriptName, pathInfo);
	// std::cout << "Script: " << scriptName << '\n';
	// std::cout << "PathInfo: " << pathInfo << '\n';
	if (_location->_hasCGIHandler(scriptName))
	{
		_isCGI = true;
		std::string	bin = _location->getCgiHandler(scriptName);
		// std::cout << "Handler: " << bin << '\n';
		CGI	handler(bin, _location->getRoot(), scriptName, pathInfo, _cl->getQueryString(), _cl->getMethod(),
			_cl->getHeader(), _ipPort, _location, _body);
		
		if (!handler.fail())
		{
			while (!handler.isDone())
			{
				;
			}
			int fd = handler.getReadPipe();
			int br;
			char	buff[1];
			_body.clear();
			do
			{
				br = read(fd, buff, 1);
				if (br > 0)
				_body.append(buff, br);
			}	while (br > 0);
		}
		_responseCode = handler.getResponseCode();
	}
}
