/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 15:24:56 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/16 16:06:59 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>
# include <vector>

# define GET_MASK(SHIFT)	(1 << SHIFT)

// Valid HTTP ErrCode are in range [100, 600[
static inline bool	_isHTTPErrorCode(const std::string &str)
{
	return ( (str.size() == 3)
		&& (str.find_first_not_of("0123456798") == std::string::npos)
		&& (str[0] >= '1' && str[0] <= '5') );
}

#endif
