/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 08:16:09 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/30 08:24:05 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands/Kick.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

Kick::Kick() {}
Kick::~Kick() {}

void Kick::execute(Server& server, Client& client, const std::vector<std::string>& params)
{
	(void)client;
	(void)params;
	server.logToServer("KICK command received", "INFO");
}