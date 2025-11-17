/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponsePost.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 16:29:42 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/17 14:53:49 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "Location.hpp"
#include "Client.hpp"

void	Response::_handlePOST(void)
{
	std::cout << "URL: " << _cl->getTargetLocation() << '\n';
	std::cout << "LOC: " << _location->getLocation() << '\n';
	std::cout << "URI: " << this->_URI << '\n';
	// if (location->)
}
