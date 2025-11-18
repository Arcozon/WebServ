/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 11:36:22 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/18 15:46:10 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"
#include "CGIEnv.hpp"

#include "IpPort.hpp"

void	CGI::_closeFd(t_fd &_fd)
{
	if (_fd >= 0)
	{
		close(_fd);
		_fd = -1;
	}
}

void	CGI::_closeFd(t_fd _fds[2])
{
	_closeFd(_fds[0]);
	_closeFd(_fds[1]);
}

CGI::CGI(const std::string &binary,
			const std::string &dirScript,
			const std::string &scriptName,
			const std::string &pathInfo,
			const std::string &queryString,
			const std::string &method,
			const t_header &header,
			const IpPort &ipPort,
			const Location *location,
			const std::string &body)
:	_binary(binary),
	_scriptPath(dirScript),
	_script(scriptName),
	_pathInfo(pathInfo),
	_queryString(queryString),
	_method(method),
	_header(header),
	_ipPort(ipPort),
	_location(location),
	_body(body),
	_pid(-1),
	_fail(false),
	_statusCode(200),
	_retVal(0)
	// TODO START TIME
{
	{
		_pipeIn[0] = -1;
		_pipeIn[1] = -1;
		_pipeOut[0] = -1;
		_pipeOut[1] = -1;
	}
	_setup();
	if (_pid == 0)
	{
		_execCGI();
	}
	else
	{
		_closeFd(_pipeIn[0]);
		_closeFd(_pipeOut[1]);
		write(_pipeIn[1], _body.c_str(), _body.size());
		std::cout << "asd";
	}
}

CGI::~CGI(void)
{	
	if (_pid != -1)
		kill(_pid, SIGKILL);
	_closeFd(_pipeIn);
	_closeFd(_pipeOut);
}

void	CGI::_setup()
{
	if (access(_binary.c_str(), X_OK) != 0
		|| access(_scriptPath.c_str(), R_OK) != 0)
	{
		_statusCode = 404;
		_fail = true;
		return ;
	}
	if (pipe(_pipeIn)  < 0 || pipe(_pipeOut) < 0)
	{
		_statusCode = 500;
		_fail = true;
		return ;
	}
	_pid = fork();
	if (_pid < 0)
	{
		_statusCode = 500;
		_fail = true;
		return ;
	}
}

bool	CGI::fail(void) const
{
	return (_fail);
}

int		CGI::getStatusCode(void) const
{
	return (_statusCode);
}
    // QUERY_STRING
    // REMOTE_HOST
    // REMOTE_ADDR
    // AUTH_TYPE
    // REMOTE_USER
    // REMOTE_IDENT
    // CONTENT_TYPE
    // CONTENT_LENGTH

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
				throw ;
			execve(_binary.c_str(), cArgv, cEnv);
		}
		catch (...)
		{
			delete[] cArgv[0];
			delete[] cArgv[1];
		}
	}
	exit(_retValServErr);
}

bool	CGI::isDone(void)
{
	return (waitpid(_pid, &_retVal))
}
