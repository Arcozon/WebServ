/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 11:36:22 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/17 16:15:46 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"
#include "CGIEnv.hpp"

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

CGI::CGI(const Location *loc, const Client *client, const std::string &URI)
:	_binary(loc->getCgiHandler(client->getTargetLocation())),
	_script(URI),
	_scriptPath(URI),
	_method(),
	_pathInfo(),
	_queryString(),
	_header(client->getHeader()),
	_pid(-1),
	_fail(false),
	_statusCode(200)
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
		// Write body TODO
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


void	CGI::_execCGI()
{
	// clean memory and fds of webserv (Clients fd and malloceds response ..) TODO
	_closeFd(_pipeIn[1]);
	_closeFd(_pipeOut[0]);
	if (dup2(_pipeIn[0], STDIN_FILENO) < 0
		|| dup2(_pipeOut[1], STDOUT_FILENO) < 0)
	{
		exit(_retValServErr);
	}
	_closeFd(_pipeIn[0]);
	_closeFd(_pipeOut[1]);
	CGI::CGIEnv::addHeader(_header);
}
