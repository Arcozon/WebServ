/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsIpPort.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:25:55 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/19 18:46:47 by gaeudes          ###   ########.fr       */
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
		enum alreadyDefined
		{
			S_host,
			S_port,
			S_clientBodySize,
			S_return
		};

		static const std::size_t	_nTabIpPort = 1;

		static const std::string	_keyHost;
		static const std::string	_keyLocalHost;
		static const std::string	_keyPort;
		static const std::string	_keyServerName;
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
	
		std::vector<std::string>			_ServerNames;
		std::string							_clientMaxBodySize;
		std::map<std::string, std::string>	_errorPages;
		std::vector<Location>				_locations;
		Return								_return;

		bool	_valid;

	private:
		inline const std::string	_inIpPort(void) const
			{
				if (_isDefined(S_host) && _isDefined(S_port))
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

		void	_addDefined(alreadyDefined toTest);
		bool	_isDefined(alreadyDefined toTest) const;

		void	printParsServ(void) const;

		static bool	_isValidNumInRange0to255(const std::string& str);
		
		bool	_isValidIP(const std::string &_hostStr) const;
		unsigned long	_IPStrToUL(const std::string &_hostStr) const;
	
		bool _isValidPort(const std::string &portStr) const;

	public:
		ParsIpPort(ParsLine &parsLine);
		~ParsIpPort(void);

		bool	_isServValid(void) const;
};

#endif
