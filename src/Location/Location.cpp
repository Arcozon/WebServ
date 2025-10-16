/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:37:11 by gaeudes           #+#    #+#             */
/*   Updated: 2025/10/16 11:05:16 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"
# include "ParsLocation.hpp"

Location::Location(void)
{}

Location::Location(std::ifstream &configFile, std::string &line)
{
	ParsLocation	parsLocation(configFile, line);

	(void)parsLocation;
}

Location::~Location(void)
{}
