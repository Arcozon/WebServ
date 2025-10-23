/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:37:11 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/16 15:05:00 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"
# include "ParsLocation.hpp"

Location::Location(void)
{}

Location::Location(ParsLine &parsLine)
{
	ParsLocation	parsLocation(parsLine);

	(void)parsLocation;
}

Location::~Location(void)
{}
