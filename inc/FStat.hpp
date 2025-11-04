/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FStat.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 16:59:20 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/04 13:35:33 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FSTAT_HPP
# define FSTAT_HPP

# include <string>

# include <sys/stat.h>
# include <unistd.h>

class FStat
{
	public:
		typedef enum FILETYPE
		{
			DIRECTORY,
			REG_FILE,
			UNKNOWN
		}	FILETYPE;

	private:
		std::string		_path;		
		enum FILETYPE	_fType;
		bool			_readable;
		
	public:
		FStat(void);
		FStat(const std::string &path, const std::string &URI = "");
		~FStat(void);

		bool	open(const std::string &path, const std::string &URI = "");

		bool	fail(void) const;
		bool	isDir(void) const;
		bool	isFile(void) const;
		bool	isAccesible(void) const;
		const char	*getPathCStr(void) const;
};

#endif
