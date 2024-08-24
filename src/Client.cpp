/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 18:20:56 by lmattern          #+#    #+#             */
/*   Updated: 2024/08/21 17:42:47 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int fd, const std::string &ip_address)
    : _fd(fd), _ip_address(ip_address), _authenticated(false) {}

int Client::getFd() const
{
    return (_fd);
}

const std::string &Client::getIpAddress() const
{
    return (_ip_address);
}

bool Client::isAuthenticated() const
{
    return (_authenticated);
}

void Client::authenticate()
{
    _authenticated = true;
}
