/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 11:37:26 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/19 17:39:58 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

# include <string>
# include <cstring>
# include <vector>
# include <map>

# include <unistd.h>
# include <sys/wait.h>

# include "Location.hpp"
# include "Client.hpp"
# include "Response.hpp"

class CGI
{
	public:
		typedef int	fd_t;
		class CGIEnv;

		typedef std::map<std::string, std::string>	t_header;

	private:
		static void	_closeFd(fd_t &_fd);
		static void	_closeFd(fd_t _fds[2]);


		static const int	_retValServErr = 255;

	private:
		const std::string	_binary;

		const std::string	_scriptPath;
		const std::string	_script;

		const std::string	_pathInfo;
		const std::string	_queryString;

		const std::string	_method;

		const t_header	_header;

		const IpPort	&_ipPort;
		const Location	*_location;

		const std::string	&_body;

		fd_t	_pipeIn[2];
		fd_t	_pipeOut[2];
		
		pid_t	_pid;
	
		// start time TODO
		bool	_fail;
		bool	_done;
		int		_statusCode;
		int		_retVal;
	
	private:
		void	_setup();

		void	_exportEnv(void) const;

		void	_execCGI();

	public:
		CGI(const std::string &binary,
			const std::string &dirScript,
			const std::string &scriptName,
			const std::string &pathInfo,
			const std::string &queryString,
			const std::string &method,
			const t_header &header,
			const IpPort &ipPort,
			const Location *location,
			const std::string &body);
		~CGI(void);

		bool	fail(void) const;
		int		getStatusCode(void) const;

		bool	isDone(void);
		fd_t	getReadPipe(void) const;
		unsigned short	getResponseCode(void) const;

};

#endif
