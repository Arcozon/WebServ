/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI_Child.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 12:57:45 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/19 13:32:04 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"
#include "CGIEnv.hpp"

void	CGI::_exportEnv(void) const
{
	CGIEnv::addVar("SERVER_SOFTWARE", "WebServ/HTTP/1.1");
	CGIEnv::addVar("SERVER_NAME", _ipPort.getHostStr());
	CGIEnv::addVar("GATEWAY_INTERFACE", "CGI/1.1");
	CGIEnv::addVar("SERVER_PROTOCOL", "HTTP/1.1");
	CGIEnv::addVar("SERVER_PORT", _ipPort.getPortStr());
	CGIEnv::addVar("REQUEST_METHOD", _method);
	CGIEnv::addVar("PATH_INFO", _pathInfo);
	CGIEnv::addVar("PATH_TRANSLATED",
		Location::simplifyLocationPath(_location->getLocation() + '/' + _pathInfo));
	CGIEnv::addVar("SCRIPT_NAME",
		Location::simplifyLocationPath(_scriptPath + '/' + _script));
	// CGIEnv::addVar("REMOTE_HOST", "HTTP/1.1");
	// CGIEnv::addVar("AUTH_TYPE", "HTTP/1.1");
	// CGIEnv::addVar("REMOTE_USER", "HTTP/1.1");
	// CGIEnv::addVar("REMOTE_IDENT", "HTTP/1.1");
	{// CGIEnv::addVar("CONTENT_TYPE", "HTTP/1.1");
		t_header::const_iterator itContType = _header.find("Content-Type");
		if (itContType != _header.end())
			CGIEnv::addVar("CONTENT_TYPE", itContType->second);
	}
	{// CGIEnv::addVar("CONTENT_LENGTH", "HTTP/1.1");
		t_header::const_iterator itContLen = _header.find("Content-Lenght");
		if (itContLen != _header.end())
			CGIEnv::addVar("CONTENT_LENGHT", itContLen->second);
		else
			CGIEnv::addVar("CONTENT_LENGHT", "");
	}
	CGI::CGIEnv::addHeader(_header);
}

void	CGI::_execCGI()
{
	// clean memory and fds of webserv (Clients fd and malloceds response ..) TODO
	_closeFd(_pipeIn[1]);
	_closeFd(_pipeOut[0]);
	if (dup2(_pipeIn[0], STDIN_FILENO) < 0
		|| dup2(_pipeOut[1], STDOUT_FILENO) < 0
		|| chdir(_scriptPath.c_str()))
		exit(_retValServErr);
	_closeFd(_pipeIn[0]);
	_closeFd(_pipeOut[1]);
	_exportEnv();
	{
		char	*cArgv[3] = {0};
		char	**cEnv;

		try
		{
			cArgv[0] = new char[_binary.size() + 1];
			std::strcpy(cArgv[0], _binary.c_str());
			cArgv[1] = new char[_script.size() + 1];
			std::strcpy(cArgv[1], _script.c_str());

			cEnv = CGIEnv::getCEnv();
			if (!cEnv)
				throw 'a';
			std::cerr << "Executing: " << _binary << ' ' << _script << '\n';
			std::cerr << "In: " <<  _scriptPath << '\n';
			execve(_binary.c_str(), cArgv, cEnv);
		}
		catch (...)
		{}
		std::cerr << "Fck\n";
		delete[] cArgv[0];
		delete[] cArgv[1];
	}
	exit(_retValServErr);
}
