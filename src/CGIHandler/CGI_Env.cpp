/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI_Env.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:31:00 by gaeudes           #+#    #+#             */
/*   Updated: 2025/12/15 15:38:49 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "CGIEnv.hpp"

std::vector<std::string>	CGI::CGIEnv::_strEnv;

void	CGI::CGIEnv::copyEnv(char *env[])
{
	_strEnv.clear();
	for (int i = 0; env[i]; ++i)
		_strEnv.push_back(std::string(env[i]));
}

char **CGI::CGIEnv::getCEnv(void)
{
	const std::size_t	envSize = _strEnv.size();
	char	**cEnv;
	try
	{
		cEnv = new char *[envSize + 1];
		
		bzero(cEnv, sizeof(cEnv) * envSize + 1);
		for (std::size_t i = 0; i < envSize; ++i)
		{
			cEnv[i] = new char[_strEnv[i].size() + 1];
			std::strcpy(cEnv[i], _strEnv[i].c_str());
		}
		cEnv[envSize] = NULL;
	}
	catch (...)
	{
		if (cEnv)
		{
			for (std::size_t i = 0; cEnv[i]; ++i)
				delete[] cEnv[i];
			delete[] cEnv;
		}
		cEnv = NULL;
	}
	return (cEnv);
}

void	CGI::CGIEnv::freeCEnv(char ** &cEnv)
{
	if (cEnv)
	{
		for (std::size_t i = 0; cEnv[i]; ++i)
			delete[] cEnv[i];
		delete[] cEnv;
	}
	cEnv = NULL;
}

void	CGI::CGIEnv::_unset(const std::string &toUnset)
{
	typedef std::vector<std::string>::iterator	VecStrIt;

	const std::string toUnsetEq(toUnset + "=");

	for (VecStrIt	it = _strEnv.begin(); it != _strEnv.end(); ++it)
	{
		if (it->compare(0, toUnsetEq.length(), toUnsetEq) == 0)
		{
			// std::cout << *it << std::endl;
			_strEnv.erase(it);
			return ;
		}
	}
}

void	CGI::CGIEnv::_addVar(std::string vName, const std::string &vContent)
{
	if (vName.empty())
		return ;
	for (int i = 0; vName[i]; ++i)
	{
		if (std::islower(vName[i]))
			vName[i] = std::toupper(vName[i]);
		else if (vName[i] == '-')
			vName[i] = '_';
	}
	_unset(vName);
	_strEnv.push_back(vName + "=" + vContent);
	// std::cerr << "[" << vName + "=" + vContent << "]\n";
}

void	CGI::CGIEnv::addVar(const std::string &vName, const std::string &vContent)
{
	_addVar(vName, vContent);
}

void	CGI::CGIEnv::printEnv(void)
{
	typedef std::vector<std::string>::const_iterator	VecStrConstIt;
	
	for (VecStrConstIt it = _strEnv.begin(); it != _strEnv.end(); ++it)
		std::cout << *it << "\n";
	std::cout << std::endl;
}

void	CGI::CGIEnv::addHeader(const std::map<std::string, std::string> &header)
{
	typedef std::map<std::string, std::string>::const_iterator	MapStrStrConstIt;
	std::string vName, vContent;

	for (MapStrStrConstIt it = header.begin(); it != header.end(); ++it)
		_addVar("HTTP_" + it->first, it->second);
}

void	CGI::CGIEnv::addVar(const std::string& varNameContent)
{
	_strEnv.push_back(varNameContent);
}
