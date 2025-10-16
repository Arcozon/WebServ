/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsLine.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 12:22:31 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/16 14:59:16 by gaeudes          ###   ########.fr       */
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
		ParsLine(void);
		~ParsLine(void);
		
		// Return false if err
		bool		readLine(const std::size_t expectedTab = 0);

		void		open(const std::string &fileName,  std::ios_base::openmode mode = (std::ios_base::openmode)8);

		static inline std::size_t	getTabDepth(const std::string &str)
		{	return (str.find_first_not_of('\t'));	};

		inline std::size_t	getTabDepth(void) const
		{	return (_tabDepth);	}

		inline const std::vector<std::string>	&getSplitLine(void) const
			{	return (_splitLine);	}
	
		inline const std::string	&getLine(void) const
			{	return (_line);	}

		inline bool	fail(void) const
			{	return (_file.fail());	}
		
};

#endif
