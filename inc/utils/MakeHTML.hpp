/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MakeHTML.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaeudes <gaeudes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 11:07:31 by gaeudes           #+#    #+#             */
/*   Updated: 2025/11/10 11:42:01 by gaeudes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAKEHTML_HPP
# define MAKEHTML_HPP

# include <stack> 
# include <string> 

# include "Response.hpp"

class Response::MakeHTML
{
	private:
		std::stack<std::string>	_markups;
		std::string				&_body;

	public:
		MakeHTML(std::string &body);
		~MakeHTML(void);

		void	addMarkup(const std::string &markupName,
							const std::string &bonusContext = "",
							const bool &toClose = true);
		bool	closeMarkup(void);

		void	addText(const std::string &text);
		void	addMarkupWithText(const std::string &markupName,
									const std::string &bonusContext,
									const std::string &text);
		void	addMarkupWithText(const std::string &markupName,
									const std::string &text);
};

#endif
