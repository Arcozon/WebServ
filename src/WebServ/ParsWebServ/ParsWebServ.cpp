/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsWebServ.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 16:08:48 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/21 14:58:19 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParsWebServ.hpp"


const std::string	Server::ParsWebServ::_keyServer("server");

static inline bool	_is_dot_config(const std::string fname)
{
	const std::string	dot_config(DOT_CONFIG);
	
	if (dot_config.size() >= fname.size())
		return (false);
	return (fname.compare(fname.size() - dot_config.size(), dot_config.size(), dot_config) == 0);
}

static const std::string	ft_basename(const std::string av0)
{
	if (av0.find_last_of('/') != std::string::npos)
		return (av0.substr(av0.find_last_of('/') + 1));
	return (av0);
}

inline void	Server::ParsWebServ::_openConfigFile(const int ac, char *av[])
{
	if (ac == 0)
		throw (MyException("Argc == 0", MyException::ELVL_FATAL));
	_pname = ft_basename(av[0]);
	if (ac < 2)
		throw (MyException("Missing config file", MyException::ELVL_FATAL, _pname));
	else if (ac > 2)
		throw (MyException("Too many args", MyException::ELVL_FATAL, _pname));
	else if (!_is_dot_config(av[1]))
		throw (MyException("File must end with " DOT_CONFIG, MyException::ELVL_FATAL, _pname));
	_parsLine.open(av[1], std::ios::in);
	if (_parsLine.fail())
		throw (MyException("Can't open config file", MyException::ELVL_FATAL, av[1]));
}

void	Server::ParsWebServ::_readConfigFile(void)
{
	while (_parsLine.readLine())
	{
		if (_parsLine.getLine() == _keyServer)
			_addServer();
		else
			throw (MyException("Unknown line in .config",
				MyException::ELVL_ERROR, _parsLine.getLine()));
	}
}

void	Server::ParsWebServ::_addServer(void)
{
	try
	{
		IpPort	newIpPort(_parsLine);
		if (newIpPort.isValid())
			_ipPorts.push_back(newIpPort);
	}
	catch (const MyException &e)
	{
		e.throwDown();
		std::cerr << e;
	}
}

Server::ParsWebServ::ParsWebServ(const int ac, char *av[])
{
	_openConfigFile(ac, av);
	_readConfigFile();
}

Server::ParsWebServ::~ParsWebServ(void)
{}

const std::vector<IpPort>	Server::ParsWebServ::getIpPorts(void) const
{
	return (_ipPorts);
}
