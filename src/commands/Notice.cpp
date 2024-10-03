/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 11:29:42 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/03 15:00:24 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network/Server.hpp"
#include "network/Client.hpp"
#include "network/Channel.hpp"
#include "commands/Command.hpp"
#include "replies.hpp"

Notice::Notice() {}
Notice::~Notice() {}

void Notice::execute(Client* client, const std::vector<std::string>& params)
{
	if (client->getStatus() != REGISTERED)
	{
		return;
	}
	if (params.size() < 2)
	{
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
	if (target.empty() || message.empty())
	{
		return;
	}
	if (target[0] == GLOBAL_CHANNEL || target[0] == LOCAL_CHANNEL)
	{
		Channel* channel = _server.getChannelByName(target);
		if (channel == NULL)
		{
			return;
		}
		std::string fullMessage = ":" + client->getPrefix() + " NOTICE " + target + " :" + message + "\r\n";
		channel->broadcast(fullMessage, client);
	}
	else
	{
		Client* recipient = _server.getClientByNickname(target);
		if (recipient == NULL)
		{
			return;
		}
		std::string fullMessage = ":" + client->getPrefix() + " NOTICE " + recipient->getNickname() + " :" + message + "\r\n";
		recipient->write(fullMessage);
	}
}
