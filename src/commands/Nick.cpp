/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 08:16:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/01 16:24:34 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/Command.hpp"
#include "network/Server.hpp"
#include "network/Client.hpp"
#include "network/Channel.hpp"
#include "replies.hpp"

Nick::Nick(Server* server) : Command(server) {}
Nick::~Nick() {}

void Nick::execute(Client& client, const std::vector<std::string>& params)
{
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
		client.setNickname(nickname);
	}
	client.reply(RPL_NICK(client.getNickname(), "new nickname set"));
}

bool Nick::isNicknameTaken(const std::string& nickname)
{
	std::vector<Client*> clients = _server->getClients();
	for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if ((*it)->getNickname() == nickname)
			return true;
	}
	return false;
}
