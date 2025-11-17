/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIEnv.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:31:25 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/17 19:59:20 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

#include <iostream>

#include <string>
#include <vector>

#include "CGI.hpp"

class CGI::CGIEnv
{
	private:
		static std::vector<std::string>		_strEnv;
		static std::vector<const char *>	_cEnv;

	private:
		static void	_addVar(std::string vName, const std::string &vContent = "");
		static void	_unset(const std::string &toUnset);

	public:
		static const char **getEnv(void);
		static char **getCEnv(void);
		static void	copyEnv(char *env[]);
		static void	printEnv(void);
		static void	addVar(const std::string& vName, const std::string &vContent);
		static void	addHeader(const std::map<std::string, std::string>& header);
};

#endif
