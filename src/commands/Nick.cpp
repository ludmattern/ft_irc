/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 08:16:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/02 16:03:56 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/Command.hpp"
#include "replies.hpp"

Nick::Nick() : Command() {}
Nick::~Nick() {}

void Nick::execute(Client& client, const std::vector<std::string>& params)
{
	if (client.getStatus() == HANDSHAKE)
	{
		client.reply(ERR_PASSWDMISMATCH(client.getNickname()));
		return;
	}
	if (params.empty())
	{
		client.reply(ERR_NONICKNAMEGIVEN(client.getNickname()));
		return;
	}

	std::string nickname = params[0];

	if (isNicknameTaken(nickname))
		client.reply(ERR_NICKNAMEINUSE(client.getNickname()));
	else
	{
		std::string oldPrefix = client.getPrefix();
		client.setNickname(nickname);
		client.write(":" + oldPrefix + " NICK :" + nickname);
	}
}


bool Nick::isNicknameTaken(const std::string& nickname)
{
	std::vector<Client*> clients = _server.getClients();
	for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if ((*it)->getNickname() == nickname)
			return true;
	}
	return false;
}
