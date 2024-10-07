/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 11:29:35 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/07 10:54:17 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/Command.hpp"
#include "network/Server.hpp"
#include "network/Client.hpp"
#include "network/Channel.hpp"
#include "replies.hpp"
#include <sstream>
#include <vector>
#include <algorithm>

Join::Join() {}
Join::~Join() {}

void Join::execute(Client* client, const std::vector<std::string>& params) 
{
	if (client->getStatus() != REGISTERED)
	{
		client->reply(ERR_NOTREGISTERED());
		return ;
	}

	if (params.empty())
	{
		client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), std::string("JOIN")));
		return ;
	}
	std::string channelName = params[0];
	std::string key;

	if (params.size() > 1)
	{
		key = params[1];
	}
	if (channelName[0] != '#' && channelName[0] != '&')
	{
		client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), channelName));
		return ;
	}
	
	Channel* channel = _server.getChannel(channelName);
	if(channel)
	{
		if (channel->hasMode('k'))
		{
			if (key.empty() || key != channel->getPassword())
			{
				client->reply(ERR_BADCHANNELKEY(client->getNickname(), channelName));
				return;
			}
		}
		if (channel->getLimit() != -1 && channel->getNumberOfClients() >= channel->getLimit()) 
		{
			client->reply(ERR_CHANNELISFULL(client->getNickname(), channel->getName()));
			return;
		}
		if (channel->hasMode('i') && !channel->isInvited(*client))
		{
			client->reply(ERR_INVITEONLYCHAN(client->getNickname(), channelName));
			return;
		}
		client->joinChannel(channel, false);
	}
	else
	{
		channel = _server.addChannel(channelName);
		channel->addClient(client, true);
		client->joinChannel(channel, true);
	}

	channel->broadcast(":" + client->getPrefix() + " JOIN " + channelName);
	channel->welcomeClient(client);
}

