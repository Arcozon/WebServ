/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 11:36:22 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/20 14:32:31 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"
#include "CGIEnv.hpp"

#include "IpPort.hpp"

void	CGI::_closeFd(fd_t &_fd)
{
	if (_fd >= 0)
	{
		close(_fd);
		_fd = -1;
	}
}

void	CGI::_closeFd(fd_t _fds[2])
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
	_done(false),
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
	if (!_fail)
	{
		if (_pid == 0)
			_execCGI();
		else
		{
			_closeFd(_pipeIn[0]);
			_closeFd(_pipeOut[1]);
			write(_pipeIn[1], _body.c_str(), _body.size());
			_closeFd(_pipeIn[1]);
		}
	}
	else
	{
		_closeFd(_pipeIn);
		_closeFd(_pipeOut);
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
	const std::string scriptLoc(_scriptPath + '/' + _script);
	
	if (access(scriptLoc.c_str(), R_OK) != 0
		|| access(_binary.c_str(), X_OK) != 0)
	{
		std::cout << "pipi\n";
		_statusCode = 404;
		_fail = true;
		return ;
	}
	if (pipe(_pipeIn) < 0
		|| pipe(_pipeOut) < 0
		|| (_pid = fork()) < 0)
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

bool	CGI::isDone(void)
{
	if (!_done && waitpid(_pid, &_retVal, WNOHANG) == _pid)
	{
		_done = true;
		if (WIFEXITED(_retVal))
			_retVal = WEXITSTATUS(_retVal);
		else if (WIFSIGNALED(_retVal))
			_retVal = WTERMSIG(_retVal);
		if (_retVal)
			_statusCode = 500;
	}
	return (_done);
}

CGI::fd_t	CGI::getReadPipe(void) const
{
	return (_pipeOut[0]);
}

unsigned short	CGI::getResponseCode(void) const
{
	return (_statusCode);
}
