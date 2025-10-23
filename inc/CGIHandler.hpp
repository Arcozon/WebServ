/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:31:25 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/22 16:26:37 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

#include <iostream>

#include <string>
#include <vector>

# define DEFAULT_GATEWAY_INTERFACE
# define DEFAULT_GATEWAY_INTERFACE

class CGIHandler
{
	private:
		static std::vector<std::string>		_strEnv;
		static std::vector<const char *>	_cEnv;

		// static std::vector pid -> request

	private:
		static const char	**getEnv(void);
		static void	_addVar(const std::string& vName, const std::string &vContent = "");

		static void	_unset(const std::string &toUnset);


		static void	_addRequestMethod(const std::string &vContent);
		static void	_addQueryString(const std::string &vContent);
		static void	_addContentType(const std::string &vContent);
		static void	_addContentLenght(const std::string &vContent);
		static void	_addScriptName(const std::string &vContent);
		static void	_addPathInfo(const std::string &vContent);
		static void	_addPathTranslated(const std::string &vContent);
		static void	_addServerName(const std::string &vContent);
		static void	_addServerPort(const std::string &vContent);
		static void	_addServerProtovol(const std::string &vContent);
		static void	_addRemoteAddr(const std::string &vContent);
		static void	_addRemotePort(const std::string &vContent);
		static void	_addGatewayInterface(const std::string &vContent = "");

	public:
		static void	copyEnv(char *env[]);
		static void	printEnv(void);
		static void	addVar(const std::string& vName, const std::string &vContent);

		static void	handleRequest(void); //(request)

		
};

#endif
