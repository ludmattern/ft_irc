/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cppLibft.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 09:34:01 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/25 09:34:27 by lmattern         ###   ########.fr       */
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