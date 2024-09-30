/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 08:17:08 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/30 08:24:23 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands/Ping.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

Ping::Ping() {}
Ping::~Ping() {}

void Ping::execute(Server& server, Client& client, const std::vector<std::string>& params)
{
	(void)client;
	(void)params;
	server.logToServer("PING command received", "INFO");
}