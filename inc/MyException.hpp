/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MyException.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 11:39:47 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/14 18:12:54 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MYEXCEPTION_HPP
# define MYEXCEPTION_HPP

# include <string>
# include <exception>

# include "colors.hpp"

class MyException : public std::exception
{
	public:
		enum eLevel {
			ELVL_WARNING,
			ELVL_ERROR,
			ELVL_FATAL,
			ELVL_MAX = 3
		};

	protected:
		static const std::string	_colorLvl[ELVL_MAX];
		static const std::string	_errLvl[ELVL_MAX];
		static const enum eLevel	_lvlThrowDown;

		const std::string	_err;
		const std::string	_context;
		const eLevel		_level;
		const std::string	_err_msg;

	public:
		MyException(const std::string &err, const eLevel level = ELVL_ERROR,
			const std::string &_context = "");
		virtual	~MyException(void) throw();	

		virtual const char	*what(void) const throw();
		const std::string	&getErrMsg(void) const throw();
		
		inline void	throwDown(void) const;
		inline eLevel	getErrLvl(void) const;
};

inline void	MyException::throwDown(void) const
{
	if (this->_level >= MyException::_lvlThrowDown)
		throw (*this);
}

inline MyException::eLevel	MyException::getErrLvl(void) const
{
	return (this->_level);
}

std::ostream	&operator<<(std::ostream &os, const MyException &exp);

#endif
