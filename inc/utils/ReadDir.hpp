/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReadDir.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 13:14:33 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/04 14:28:50 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef READDIR_HPP
# define READDIR_HPP

# include <string>

# include <sys/types.h>
# include <dirent.h>

# include "FStat.hpp"

class ReadDir
{
	private:
		DIR		*_dir;
		dirent	*_dirEnt;

	public:
		ReadDir(const std::string str);
		ReadDir(const char cStr[]);
		~ReadDir(void);

		std::string	getNextFile(void);
		FStat::FILETYPE		getFileType(void) const;
		
		bool	fail(void) const;

};


#endif
