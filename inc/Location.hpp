/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 16:18:20 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/25 14:53:32 by gaeudes          ###   ########.fr       */
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

	public:
		enum sAllowedMethods
		{
			s_GET = 0,
			s_POST,
			s_DELETE,
			s_METHODS_MAX
		};
		enum sDefined
		{
			s_root = s_METHODS_MAX,
			s_autoindex,
			s_upload_store,
			s_return,
			s_allow
		};

	private:
		unsigned long	_flags;

		// std::string		_guard;	??
		std::string		_location;
		std::string		_root;
		std::vector<std::string>	_index;
		bool			_autoindex;
		std::map<std::string, std::string>	_cgiHandler;
		std::map<std::string, std::string>	_errPages;
		Return								_return;

	public:
		Location(void);
		Location(ParsLine &parsLine);
		~Location(void);

		bool	_isAllowedGET(void) const;
		bool	_isAllowedPOST(void) const;
		bool	_isAllowedDELETE(void) const;
};

#endif
