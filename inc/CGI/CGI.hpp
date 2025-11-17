/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 11:37:26 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/17 11:54:51 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

# include <string>
# include <vector>

# include <unistd.h>

# include "Location.hpp"
# include "Client.hpp"

class CGI
{
	public:
		typedef int	t_fd;
		class CGIEnv;

	private:
		static void	_closeFd(t_fd &_fd);
		static void	_closeFd(t_fd _fds[2]);

	private:
		const std::string	_binary;
		const std::string	_script;

		const std::string	_method;

		t_fd	_pipeIn[2];
		t_fd	_pipeOut[2];
		
		pid_t	_pid;
	
	public:
		CGI(const Location &loc, const Client &client);
		~CGI(void);	
};

#endif
