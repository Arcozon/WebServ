/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Return.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 14:20:21 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/10 18:09:29 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RETURN_HPP
# define RETURN_HPP

# include <string>
# include <algorithm>

# include "MyException.hpp"
# include "utils.hpp"

class	Return
{
	private:
		enum typeReturn
		{
			CODE,
			CODE_URL,
			CODE_BODY,
			UNDEF
		};

	public:
		// Valid HTTP ErrCode are in range [100, 600[		
		static bool	_isHTTPErrorCode(const int code);
		static bool	_isHTTPErrorCode(const std::string &code);

	private:
		typeReturn	_type;
		std::string	_argOne;
		std::string	_argTwo;
	
	public:
		Return(const std::string &code, const std::string &url);
		Return(const std::string &text);
		Return(void);
		~Return(void);

		Return	&operator=(const Return &other);

		const std::string	&getCodeStr(void) const;
		int					getCode(void) const;
		const std::string	&getArgTwo(void) const;

		bool	isCode(void) const;
		bool	isRedirect(void) const;
		bool	isBody(void) const;

		void	_printInfo(void) const;
		// void	_fixUrlWithDomainName(const std::string &domain, const std::string &location);
		void	_fixUrlWithDomainName(const std::string &domain);
};

#endif
