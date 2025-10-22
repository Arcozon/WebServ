/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:31:25 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/22 14:58:22 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

#include <iostream>

#include <string>
#include <vector>

class CGIHandler
{
	private:
		static std::vector<std::string>		_strEnv;
		static std::vector<const char *>	_cEnv;

	private:
		static const char	**getEnv(void);
		static void	_addVar(const std::string& vName, const std::string &vContent = "");

	public:
		static void	copyEnv(char *env[]);
		static void	printEnv(void);
		static void	addVar(const std::string& vName, const std::string &vContent);
};

#endif
