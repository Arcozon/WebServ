/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIEnv.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:31:25 by gaeudes           #+#    #+#             */
/*   Updated: 2025/12/15 14:43:18 by gaeudes          ###   ########.fr       */
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

	private:
		static void	_addVar(std::string vName, const std::string &vContent = "");
		static void	_unset(const std::string &toUnset);

	public:
		static void	copyEnv(char *env[]);
		static void	printEnv(void);
		static void	addVar(const std::string& vName, const std::string &vContent);
		static void	addVar(const std::string& varNameContent);
		static void	addHeader(const std::map<std::string, std::string>& header);
		
		static char	**getCEnv(void);
		static void	freeCEnv(char ** &cEnv);
};

#endif
