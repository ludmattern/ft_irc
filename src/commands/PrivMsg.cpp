/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivMsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 11:29:45 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/06 16:43:50 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network/Server.hpp"
#include "network/Client.hpp"
#include "network/Channel.hpp"
#include "commands/Command.hpp"
#include "replies.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <bits/basic_string.h>
#include <cstdlib>

PrivMsg::PrivMsg() {}
PrivMsg::~PrivMsg() {}

void PrivMsg::execute(Client* client, const std::vector<std::string> &params)
{
	if (client->getStatus() != REGISTERED)
	{
		client->reply(ERR_NOTREGISTERED());
		return;
	}

	if (params.size() < 2)
	{
		client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), std::string("PRIVMSG")));
		return;
	}

	std::string target = params[0];
	std::string message;

	if (params[1][0] == ':')
	{
		message = params[1].substr(1);
		for (size_t i = 2; i < params.size(); ++i)
			message += " " + params[i];
	}
	else
	{
		for (size_t i = 1; i < params.size(); ++i)
		{
			if (i != 1)
				message += " ";
			message += params[i];
		}
	}

	if (target.empty())
	{
		client->reply(ERR_NORECIPIENT(client->getNickname(), "PRIVMSG"));
		return;
	}

	if (message.empty())
	{
		client->reply(ERR_NOTEXTTOSEND(client->getNickname()));
		return;
	}

	if (target[0] == GLOBAL_CHANNEL || target[0] == LOCAL_CHANNEL)
	{
		Channel* channel = _server.getChannel(target);
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
		Client* recipient = _server.getClientByNickname(target);
		if (recipient == NULL)
		{
			client->reply(ERR_NOSUCHNICK(target));
			return;
		}

		std::string fullMessage = ":" + client->getPrefix() + " PRIVMSG " + recipient->getNickname() + " :" + message;
		recipient->write(fullMessage);
	}
}
