/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsLocation.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:03:48 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/14 18:11:43 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSLOCATION_HPP
# define PARSLOCATION_HPP

# include "Location.hpp"
# include "MyException.hpp"

# define GET_MASK(SHIFT)	(1 << SHIFT)

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
			S_return,
			S_cookie_enable // ?
		};
		enum allowedMethods
		{
			S_GET,
			S_POST,
			S_DELETE
		};

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
};

#endif
