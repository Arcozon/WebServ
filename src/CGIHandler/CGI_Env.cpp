/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI_Env.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:31:00 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/17 16:13:23 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "CGIEnv.hpp"

std::vector<std::string>	CGI::CGIEnv::_strEnv;
std::vector<const char *>	CGI::CGIEnv::_cEnv;

void	CGI::CGIEnv::copyEnv(char *env[])
{
	_strEnv.clear();
	for (int i = 0; env[i]; ++i)
		_strEnv.push_back(std::string(env[i]));
}

const char	**CGI::CGIEnv::getEnv(void)
{
	typedef std::vector<std::string>::const_iterator	VecStrConstIt;

	_cEnv.clear();
	for (VecStrConstIt it = _strEnv.begin(); it != _strEnv.end(); ++it)
		_cEnv.push_back(it->c_str());
	_cEnv.push_back(NULL);
	return (_cEnv.data());
}

void	CGI::CGIEnv::_unset(const std::string &toUnset)
{
	typedef std::vector<std::string>::iterator	VecStrIt;

	const std::string toUnsetEq(toUnset + "=");

	for (VecStrIt	it = _strEnv.begin(); it != _strEnv.end(); ++it)
	{
		if (it->compare(0, toUnsetEq.length(), toUnsetEq) == 0)
		{
			std::cout << *it << std::endl;
			_strEnv.erase(it);
			return ;
		}
	}
}

void	CGI::CGIEnv::_addVar(const std::string &vName, const std::string &vContent)
{
	if (vName.empty())
		return ;
	
	std::string envVar = vName + "=" + vContent;
	_unset(vName);
	_strEnv.push_back(envVar);
}

void	CGI::CGIEnv::addVar(const std::string &vName, const std::string &vContent)
{
	_addVar(vName, vContent);
}

void	CGI::CGIEnv::printEnv(void)
{
	const char	**cEnv = getEnv();

	std::cout << "Env:\n";
	for (int i = 0; cEnv[i]; ++i)
		std::cout << cEnv[i] << "\n";
	std::cout << std::endl;
}

void	CGI::CGIEnv::addHeader(const std::map<std::string, std::string> &header)
{
	typedef std::map<std::string, std::string>::const_iterator	MapStrStrConstIt;
	std::string vName, vContent;

	for (MapStrStrConstIt it = header.begin(); it != header.end(); ++it)
	{
		vName = it->first;
		vContent = it->second;
		for (int i = 0; vName[i]; ++i)
		{
			if (std::isupper(vName[i]))
				vName[i] = std::toupper(vName[i]);
			else if (vName[i] == '-')
				vName[i] = '_';
			_addVar(vName, vContent);
		}
	}
}
