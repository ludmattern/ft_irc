/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivMsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 11:29:45 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/03 16:15:22 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network/Server.hpp"
#include "network/Client.hpp"
#include "network/Channel.hpp"
#include "commands/Command.hpp"
#include "replies.hpp"

PrivMsg::PrivMsg() {}
PrivMsg::~PrivMsg() {}

void PrivMsg::execute(Client* client, const std::vector<std::string> &params)
{
	if (client->getStatus() != REGISTERED)
	{
		client->reply(ERR_NOTREGISTERED(client->getNickname()));
		return;
	}

	if (params.size() < 2)
	{
		client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "PRIVMSG"));
		return;
	}

	std::string target = params[0];
	std::string message;

	for (size_t i = 1; i < params.size(); ++i)
	{
		if (i == 1 && params[i][0] == ':')
			message += params[i].substr(1);
		else
			message += " " + params[i];
	}
	if (target.empty())
	{
		client->reply("NO TARGET");
		return;
	}
	if (message.empty())
	{
		client->reply("NOTHING TO SEND");
		return;
	}
	if (target[0] == GLOBAL_CHANNEL || target[0] == LOCAL_CHANNEL)
	{
		Channel* channel = Server::getInstance().getChannelByName(target);
		if (channel == NULL)
		{
			client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), target));
			return;
		}
		if (!channel->hasClient(client))
		{
			client->reply(ERR_CANNOTSENDTOCHAN(client->getNickname(), target));
			return;
		}
		std::string fullMessage = ":" + client->getPrefix() + " PRIVMSG " + target + " :" + message;
		channel->broadcast(fullMessage, client);
	}
	else
	{
		Client* recipient = Server::getInstance().getClientByNickname(target);
		if (recipient == NULL)
		{
			client->reply(ERR_NOSUCHNICK(client->getNickname(), target));
			return;
		}
		std::string fullMessage = ":" + client->getPrefix() + " PRIVMSG " + recipient->getNickname() + " :" + message;
		recipient->write(fullMessage);
	}
}
