/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 17:26:22 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/02 16:53:19 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network/Server.hpp"
#include "commands/Command.hpp"
#include "replies.hpp"

Command::Command() : _server(Server::getInstance()) {}

void Command::tryRegister(Client &client) 
{
	if (client.getPass() != _server.getPassword())
	{
		client.reply(ERR_PASSWDMISMATCH(client.getNickname()));
		return;
	}
	client.setStatus(REGISTERED);
}
