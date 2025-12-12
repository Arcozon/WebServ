/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsLine.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 12:28:11 by gaeudes           #+#    #+#             */
/*   Updated: 2025/12/12 17:56:15 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParsLine.hpp"

ParsLine::ParsLine(void)
:	_skipComments(true),
	_needNewline(true),
	_tabDepth(0)
{}

ParsLine::ParsLine(const std::string &fileName, const bool &skipComments)
:	_skipComments(skipComments),
	_needNewline(true),
	_tabDepth(0)
{
	_file.open(fileName.c_str());
}

ParsLine::~ParsLine(void)
{
	_file.close();
}

void	ParsLine::open(const std::string &fileName, std::ios_base::openmode mode)
{
	_file.close();
	_file.open(fileName.c_str(), mode);
}

bool	ParsLine::_isLineComment(void) const
{
	return (_skipComments && !_line.empty() && _line[0] == _cComment);
}

bool	ParsLine::_isLineEmpty(void) const
{
	return (_line.empty() || getTabDepth(_line) == std::string::npos);
}

bool	ParsLine::readLine(const std::size_t expectedTab)
{
	if (_file.fail())
	{
		return (false);
	}
	else if (_needNewline)
	{
		do
		{
			if (!std::getline(_file, _line))
				return (false);
		}	while (_isLineEmpty() || _isLineComment());
		_tabDepth = getTabDepth(_line);
		splitLine();
	}
	_needNewline = ( _tabDepth >= expectedTab );
	return (_needNewline);
}

void	ParsLine::splitLine(const char sep, const bool skipTabs)
{
	_splitLine = ParsLine::splitLine(_line, sep, skipTabs);
}

std::vector<std::string>	ParsLine::splitLine(const std::string &str,
	const char sep, const bool skipTabs)
{
	std::vector<std::string>	split;
	std::size_t	nextSpace = std::string::npos;
	std::size_t	start = 0;

	if (skipTabs)
		start = getTabDepth(str);
	start = str.find_first_not_of(sep, start);
	while (start != std::string::npos)
	{
		nextSpace = str.find_first_of(sep, start);
		split.push_back(str.substr(start, nextSpace - start));
		start = str.find_first_not_of(sep, nextSpace);
	}
	return (split);
}
