/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:31:00 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/22 15:03:52 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "CGIHandler.hpp"

std::vector<std::string>	CGIHandler::_strEnv;
std::vector<const char *>	CGIHandler::_cEnv;

void	CGIHandler::copyEnv(char *env[])
{
	_strEnv.clear();
	for (int i = 0; env[i]; ++i)
		_strEnv.push_back(std::string(env[i]));
}

const char	**CGIHandler::getEnv(void)
{
	typedef std::vector<std::string>::iterator	vsit_t;

	_cEnv.clear();
	for (vsit_t it = _strEnv.begin(); it != _strEnv.end(); ++it)
		_cEnv.push_back(it->c_str());
	_cEnv.push_back(NULL);
	return (_cEnv.data());
}

void	CGIHandler::_addVar(const std::string &vName, const std::string &vContent)
{
	if (vName.empty())
		return ;
	
	std::string envVar = vName + "=" + vContent;

	_strEnv.push_back(envVar);
}

void	CGIHandler::addVar(const std::string &vName, const std::string &vContent)
{
	_addVar(vName, vContent);
}

void	CGIHandler::printEnv(void)
{
	const char	**cEnv = getEnv();

	std::cout << "Env:\n";
	for (int i = 0; cEnv[i]; ++i)
		std::cout << cEnv[i] << "\n";
	std::cout << std::endl;
}
