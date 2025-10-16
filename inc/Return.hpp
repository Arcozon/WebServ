/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Return.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 14:20:21 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/16 15:27:05 by gaeudes          ###   ########.fr       */
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
			RET_TEXT,
			RET_ERR_URL
		};

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
};

#endif
