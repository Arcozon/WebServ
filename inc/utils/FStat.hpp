/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FStat.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 16:59:20 by gaeudes           #+#    #+#             */
/*   Updated: 2025/12/12 18:15:15 by gaeudes          ###   ########.fr       */
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
		enum FILETYPE
		{
			DIRECTORY,
			REG_FILE,
			UNKNOWN,
			ERR_STAT
		};

	private:
		std::string		_path;		
		enum FILETYPE	_fType;
		bool			_readable;
		bool			_writable;
		
	public:
		FStat(void);
		FStat(const std::string &path);
		FStat(const std::string &path, const std::string &URI);
		~FStat(void);

		bool	open(const std::string &path);
		bool	open(const std::string &path, const std::string &URI);

		bool	fail(void) const;
		bool	isDir(void) const;
		bool	isFile(void) const;
		bool	isReadable(void) const;
		bool	isWritable(void) const;
		const char	*getPathCStr(void) const;
};

#endif
