/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileDir.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 16:59:20 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/03 17:50:54 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILEDIR_HPP
# define FILEDIR_HPP

# include <string>

# include <sys/stat.h>
# include <unistd.h>

class FileDir
{
	private:
		enum FILETYPE
		{
			DIRECTORY,
			REG_FILE,
			UNKMOWN
		};

	private:
		const std::string	_path;		
		enum FILETYPE		_fType;
		bool				_readable;
		
	public:
		FileDir(const std::string &path, const std::string &URI = "");
		~FileDir(void);

		bool	fail(void) const;
		bool	isDir(void) const;
		bool	isFile(void) const;
		bool	isAccesible(void) const;
		const char	*getPathCStr(void) const;
};

#endif
