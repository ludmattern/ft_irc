/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 18:20:56 by lmattern          #+#    #+#             */
/*   Updated: 2024/08/20 18:21:31 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Channel.hpp"

Channel::Channel() {
}

Channel::Channel(const Channel& other) {
    memberVar = other.memberVar;
}

Channel& Channel::operator=(const Channel& other) {
    if (this != &other) {
        memberVar = other.memberVar;
    }
    return *this;
}

Channel::~Channel() {
}