/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsLocation.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:03:48 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/15 15:35:23 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSLOCATION_HPP
# define PARSLOCATION_HPP

# include "Location.hpp"
# include "MyException.hpp"

# include "utils.hpp"

class Location::ParsLocation
{
	private:
		enum alreadyDefined
		{
			S_root,
			S_allowed_methods,
			S_autoindex,
			S_upload_store,
			S_upload,
			S_return
		};
		enum allowedMethods
		{
			S_GET,
			S_POST,
			S_DELETE
		};

		static bool	_isOnOff(const std::string &str);

		static const unsigned int	_nTabLocation = 2;

	private:
		unsigned long	_defined;

		unsigned char	_allowed;
		std::string		_root;
		std::string		_index;
		std::string		_guard;
		bool			_autoindex;
		std::map<std::string, std::string>	_cgiHandler;
		std::map<std::string, std::string>	_errPages;

	public:
		ParsLocation(void);
		~ParsLocation(void);

		bool	_isParsLocationValid(void) const;

		bool	_addLine(const std::string &line);
};

#endif
