/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 08:15:02 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/30 08:24:07 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands/Invite.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

Invite::Invite() {}
Invite::~Invite() {}

void Invite::execute(Server& server, Client& client, const std::vector<std::string>& params)
{
	(void)client;
	(void)params;
	server.logToServer("INVITE command received", "INFO");
}
