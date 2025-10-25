/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IpPort.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 12:03:09 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/25 15:28:20 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IPPORT_HPP
# define IPPORT_HPP

# include <string>
# include <vector>
# include <map>

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

		std::vector<std::string>			_IpPortNames;
		std::map<std::string, std::string>	_errPages;

		unsigned long	_clientMaxBodySize;

		std::vector<Location>	_locations;

	public:
		IpPort(void);
		IpPort(ParsLine &parsLine);
		~IpPort(void);
};

#endif
