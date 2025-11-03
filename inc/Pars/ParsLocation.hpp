/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsLocation.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:03:48 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/03 10:55:35 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSLOCATION_HPP
# define PARSLOCATION_HPP

# include <iostream>
# include <fstream>

# include <string>
# include <vector>

# include "Location.hpp"
# include "Return.hpp"
# include "MyException.hpp"

# include "ParsLine.hpp"
# include "utils.hpp"

class Location::ParsLocation
{
	private:
		static bool	_isOnOff(const std::string &str);

		static const std::size_t	_nTabLocation = 2;
		static const bool			_defaultAutoIndex = false;

		static const std::string	_keyRoot;
		static const std::string	_keyIndex;
		static const std::string	_keyAutoIndex;
		static const std::string	_keyAllow;
		static const std::string	_keyUploadLocation;
		static const std::string	_keyReturn;
		static const std::string	_keyCGIHandler;
		static const std::string	_keyErrorPage;

		static const std::string	_keyAllowGet;
		static const std::string	_keyAllowPost;
		static const std::string	_keyAllowDelete;

	private:
		ParsLine	&_parsLine;	

		unsigned long	_fDefined;

		std::string					_location;
		std::string					_root;
		std::vector<std::string>	_index;
		bool						_autoindex;
		unsigned long				_allow;
		std::string					_uploadLocation;
		Return						_return;
		std::map<std::string, std::string>	_cgiHandler;
		std::map<std::string, std::string>	_errorPages;

		bool	_valid;

	private:
		inline const std::string	_inLocation(void) const
			{	return (std::string(" in location " + _location));	}

		bool	_isAllowed(sAllowedMethods toTest) const;
		bool	_isDefined(sDefined toTest) const;
		void	_addDefined(sDefined toTest);

		void	_addLocationLine(void);
		void	_addRoot(void);
		void	_addIndex(void);
		void	_addAutoIndex(void);
		void	_addAllow(void);
		void	_addUploadLocation(void);
		void	_addReturn(void);
		void	_addCGIHandler(void);
		void	_addErrPages(void);

		bool _checkRedirs(void) const;

	public:
		ParsLocation(ParsLine &parsLine);
		~ParsLocation(void);

		bool		isParsLocationValid(void) const;
		
		void		printfLocation(void) const;
		
		unsigned long		getLocationFlags(void) const;
		const std::string	&getLocation(void) const;
		const std::string	&getRoot(void) const;
		const std::vector<std::string>	&getIndex(void) const;
		const bool			&getAutoIndex(void) const;
		const std::map<std::string, std::string>	&getCGIHandler(void) const;
		const std::map<std::string, std::string>	&getErrPages(void) const;
		const Return		&getReturn(void) const;
};

#endif
