/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 18:18:09 by lmattern          #+#    #+#             */
/*   Updated: 2024/08/20 18:18:12 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() {
}

Server::Server(const Server& other) {
    memberVar = other.memberVar;
}

Server& Server::operator=(const Server& other) {
    if (this != &other) {
        memberVar = other.memberVar;
    }
    return *this;
}

Server::~Server() {
}