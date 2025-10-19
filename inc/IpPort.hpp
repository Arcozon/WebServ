/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IpPort.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 12:03:09 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/19 11:14:57 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IPPORT_HPP
# define IPPORT_HPP

# include <string>
# include <vector>

# include "Location.hpp"
# include "Return.hpp"

# include "ParsLine.hpp"

class IpPort
{
	private:
		class ParsIpPort;

	private:
		unsigned long	_fDefined;

		std::string		_host;
		std::string		_port;
		std::string		_url;

		std::vector<std::string>			_IpPortNames;
		std::map<std::string, std::string>	_errPages;

		unsigned long	_maxBodySize;

		std::vector<Location>	_locations;

	public:
		IpPort(void);
		IpPort(ParsLine &parsLine);
		~IpPort(void);
};

#endif
