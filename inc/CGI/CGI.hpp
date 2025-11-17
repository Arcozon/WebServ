/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 11:37:26 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/17 16:14:51 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

# include <string>
# include <vector>
# include <map>

# include <unistd.h>

# include "Location.hpp"
# include "Client.hpp"
# include "Response.hpp"

class CGI
{
	public:
		typedef int	t_fd;
		class CGIEnv;

		typedef std::map<std::string, std::string>	t_header;

	private:
		static void	_closeFd(t_fd &_fd);
		static void	_closeFd(t_fd _fds[2]);


		static const int	_retValServErr = 255;

	private:
		const std::string	_binary;
		const std::string	_script;
		const std::string	_scriptPath;

		const std::string	_method;
		const std::string	_pathInfo;
		const std::string	_queryString;

		const t_header	_header;

		t_fd	_pipeIn[2];
		t_fd	_pipeOut[2];
		
		pid_t	_pid;
	
		// start time
		bool	_fail;
		int		_statusCode;
	
	private:
		void	_setup();

		void	_execCGI();

	public:
		CGI(const Location *loc, const Client *client, const std::string &URI);
		~CGI(void);

		bool	fail(void) const;
		int		getStatusCode(void) const;

		bool	isDone(void);
};

#endif
