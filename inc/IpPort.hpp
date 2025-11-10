/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IpPort.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 12:03:09 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/10 18:00:57 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IPPORT_HPP
# define IPPORT_HPP

# include <string>
# include <vector>
# include <map>
# include <sstream>
# include <algorithm>

# include "Location.hpp"
# include "Return.hpp"

# include "ParsLine.hpp"

class IpPort
{
	private:
		class ParsIpPort;

	public:
		enum sDefined
		{
			s_host,
			s_port,
			s_clientBodySize,
			s_return
		};
	private:
		unsigned long	_fDefined;

		std::string		_StrHost;
		unsigned long	_host;
		std::string		_StrPort;
		unsigned short	_port;
		std::string		_strHostPort;

		Return			_return;
		
		std::map<std::string, std::string>	_errPages;

		unsigned long	_clientMaxBodySize;

		std::vector<Location>	_locations;

		bool			_valid;

	public:
		IpPort(void);
		IpPort(ParsLine &parsLine);
		~IpPort(void);

		bool	isValid(void) const;

		const std::string	&getIpPortStr(void) const;
		const std::string &getHostStr(void) const;
		const std::string &getPortStr(void) const;
		const unsigned long &getHost(void) const;
		const unsigned short &getPort(void) const;

		bool errorPageDefined(const std::string &errCode) const;
		bool errorPageDefined(const std::size_t &errCode) const;
		const std::string	&getErrorPage(const std::string &errCode) const;
		const std::string	&getErrorPage(const std::size_t &errCode) const;
		
		const Location	*getLocation(const std::string &cPath)	const;
};

#endif
