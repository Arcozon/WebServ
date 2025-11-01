/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsIpPort.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:25:55 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/01 17:48:01 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSIPPORT_HPP
# define PARSIPPORT_HPP

# include <iostream>
# include <fstream>
# include <string>
# include <vector>
# include <map>
# include <utility>

# include "IpPort.hpp"
# include "Location.hpp"
# include "MyException.hpp"

# include "ParsLine.hpp"
# include "utils.hpp"

class IpPort::ParsIpPort
{
	private:
		static const std::size_t	_nTabIpPort = 1;

		static const std::string	_keyHost;
		static const std::string	_keyLocalHost;
		static const std::string	_keyPort;
		static const std::string	_keyClientBodySize;
		static const std::string	_keyErrorPage;
		static const std::string	_keyLocation;
		static const std::string	_keyReturn;

	private:
		ParsLine	&_parsLine;

		unsigned long	_fDefined;
	
		std::string							_hostStr;
		unsigned long						_host;
		std::string							_portStr;
		short								_port;
	
		unsigned long						_clientMaxBodySize;
		std::map<std::string, std::string>	_errorPages;
		std::vector<Location>				_locations;
		Return								_return;

		bool	_valid;

	private:
		inline const std::string	_inIpPort(void) const
			{
				if (_isDefined(s_host) && _isDefined(s_port))
					return (" in IpPort " + _hostStr + ":" + _portStr);
				return (" in IpPort");
			}

		void	_addIpPortLine(void);
		void	_addHost(void);
		void	_addPort(void);
		void	_addIpPortName(void);
		void	_addClientBodySize(void);
		void	_addErrorPage(void);
		void	_addLocation(void);
		void	_addReturn(void);

		void	_addDefined(sDefined toTest);
		bool	_isDefined(sDefined toTest) const;

		void	printParsServ(void) const;

		static bool	_isValidNumInRange0to255(const std::string& str);
		
		bool	_isValidIP(const std::string &_hostStr) const;
		unsigned long	_IPStrToUL(const std::string &_hostStr) const;
	
		bool _isValidPort(const std::string &portStr) const;

	public:
		ParsIpPort(ParsLine &parsLine);
		~ParsIpPort(void);

		bool	_isIpPortValid(void) const;

		unsigned long		getIpPortFlag(void) const;
		const std::string	&getStrHost(void) const;
		unsigned long		getHost(void) const;
		const std::string	&getStrPort(void) const;
		unsigned long		getPort(void) const;
		const std::vector<std::string>	&getServerNames(void) const;
		unsigned long					getClientMaxBodySize(void) const;
		const std::map<std::string, std::string>	&getErrorPages(void) const;
		const std::vector<Location>		&getLocations(void) const;
		const Return					&getReturn(void) const;
};

#endif
