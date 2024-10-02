/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 17:26:22 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/03 01:16:58 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network/Server.hpp"
#include "commands/Command.hpp"
#include "replies.hpp"

Command::Command() : _server(Server::getInstance()) {}

void Command::tryRegister(Client& client) 
{
	if (!client.isAuthenticated())
		return;

	if (client.getNickname().empty())
		return;

	if (client.getUsername().empty())
		return;

	client.setStatus(REGISTERED);
	client.reply(RPL_WELCOME(client.getNickname()));
}