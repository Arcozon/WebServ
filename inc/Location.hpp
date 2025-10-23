/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 16:18:20 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/23 18:13:12 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <string>
# include <map>

# include "MyException.hpp"
# include "Return.hpp"

# include "ParsLine.hpp"

class Location
{
	private:
		class ParsLocation;

	private:
		unsigned long	_flags;

		std::string		_location;
		std::string		_root;
		std::string		_index;
		std::string		_guard;
		bool			_autoindex;
		std::map<std::string, std::string>	_cgiHandler;
		std::map<std::string, std::string>	_errPages;
		Return								_return;

	public:
		Location(void);
		Location(ParsLine &parsLine);
		~Location(void);

		bool	_isGET(void) const;
		bool	_isPOST(void) const;
		bool	_isDELETE(void) const;
};

#endif
