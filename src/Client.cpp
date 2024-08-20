/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 18:20:56 by lmattern          #+#    #+#             */
/*   Updated: 2024/08/20 18:21:12 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() {
}

Client::Client(const Client& other) {
    memberVar = other.memberVar;
}

Client& Client::operator=(const Client& other) {
    if (this != &other) {
        memberVar = other.memberVar;
    }
    return *this;
}

Client::~Client() {
}