/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cppLibft.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 09:34:01 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/01 16:04:18 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CPPLIBFT_HPP
#define CPPLIBFT_HPP

#include <sstream>

template <typename T>
std::string toString(const T& value)
{
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

#endif