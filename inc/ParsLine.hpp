/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsLine.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 12:22:31 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/16 12:38:36 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSLINE_HPP
# define PARSLINE_HPP

# include <fstream>

# include <vector>
# include <string>

# include "utils.hpp"

class ParsLine
{
	private:
		std::ifstream	_file;
		std::string		_line;
		std::vector<std::string>		_splitLine;

		bool		_needNewline;
		std::size_t	_tabDepth;
	
	private:
		bool	_isLineEmpty(void);

	public:
		ParsLine(const std::string &fileName);
		~ParsLine(void);
		
		bool		readLine(void);

		std::size_t	getTabDepth(void) const;
		const std::vector<std::string>	&getSplitLine(void) const;
		const std::string	&getLine(void) const;

		bool	fail(void) const;

		static inline std::size_t	getTabDepth(const std::string &str)
		{
			return (str.find_first_not_of('\t'));
		};
};

#endif
