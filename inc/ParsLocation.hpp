/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsLocation.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:03:48 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/16 12:13:29 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSLOCATION_HPP
# define PARSLOCATION_HPP

# include <fstream>

# include <string>
# include <vector>

# include "Location.hpp"
# include "Return.hpp"
# include "MyException.hpp"

# include "utils.hpp"

class Location::ParsLocation
{
	private:
		enum alreadyDefined
		{
			S_root,
			S_autoindex,
			S_allow,
			S_upload_store,
			S_return
		};
		enum allowMethods
		{
			S_GET,
			S_POST,
			S_DELETE,
			S_METHODS_MAX
		};

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

	private:
		std::ifstream				&_configFile;
		std::string					&_line;
		std::vector<std::string>	_splitLine;

		unsigned long	_fDefined;

		std::string					_location;
		std::string					_root;
		std::vector<std::string>	_index;
		bool						_autoindex;
		unsigned char				_allow;
		std::string					_uploadLocation;
		Return						_return;
		std::map<std::string, std::string>	_cgiHandler;
		std::map<std::string, std::string>	_errorPages;

	private:
		bool	_isDefined(alreadyDefined toTest) const;
		void	_addDefined(alreadyDefined toTest);

		void	_addLocationLine(void);
		void	_addRoot(void);
		void	_addIndex(void);
		void	_addAutoIndex(void);
		void	_addAllow(void);
		void	_addUploadLocation(void);
		void	_addReturn(void);
		void	_addCGIHandler(void);
		void	_addErrPages(void);

	public:
		ParsLocation(std::ifstream &configFile, std::string &line);
		~ParsLocation(void);

		bool	isParsLocationValid(void) const;

		Location	toLocation(void) const;
};

#endif
