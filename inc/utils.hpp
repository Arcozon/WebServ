/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 15:24:56 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/15 16:12:22 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>
# include <vector>

# define GET_MASK(SHIFT)	(1 << SHIFT)

// Valid HTTP ErrCode are in range [100, 600[
static inline bool	_isHTTPErrorCode(const std::string &str)
{
	return ( (str.size() == 3)
		&& (str.find_first_not_of("0123456798") == std::string::npos)
		&& (str[0] >= '1' && str[0] <= '5') );
}

// Number of tabs at the start of line
static inline std::size_t	_getTabDepth(const std::string &str)
{
	return (str.find_first_not_of('\t'));
}
#include <iostream>
// Splits a string into a vector of substrings separated by sep(' ')
static inline std::vector<std::string>	_split(const std::string &str,
	const char sep = ' ', const bool skipTabs = true)
{
	std::vector<std::string>	split;
	
	std::size_t	nextSpace = std::string::npos;
	std::size_t	start = 0;
	if (skipTabs)
		start = _getTabDepth(str);
	while (start != std::string::npos)
	{
		nextSpace = str.find_first_of(sep, start);
		split.push_back(str.substr(start, nextSpace - start));
		start = str.find_first_not_of(sep, nextSpace);
	}
	if (nextSpace != std::string::npos)
		split.push_back("");
	return (split);
}

// Splits a string into a vector of substrings separated by ' '
static inline std::vector<std::string>	_split(const std::string &str,
	const bool skipTabs)
{
	return (_split(str, ' ', skipTabs));
}

static inline bool	_isLineEmpty(const std::string &str)
{
	return (str.empty() || _getTabDepth(str) == std::string::npos);
}

#endif
