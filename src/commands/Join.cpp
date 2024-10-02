/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 11:29:35 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/02 14:05:08 by lmattern         ###   ########.fr       */
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

void Join::execute(Client& client, const std::vector<std::string>& params) 
{
	if (client.getStatus() != REGISTERED)
	{
		client.reply(ERR_NOTREGISTERED(client.getNickname()));
		return ;
	}

	if (params.empty())
	{
		client.reply(ERR_NEEDMOREPARAMS(client.getNickname(), "JOIN"));
		return ;
	}

	std::string channelName = params[0];

	if (channelName[0] != '#' && channelName[0] != '&')
	{
		client.reply(ERR_NOSUCHCHANNEL(client.getNickname(), channelName));
		return ;
	}

	Channel* channel = _server.getChannel(channelName);
	if(channel)
		channel->addClient(client);
	else
	{
		_server.addChannel(channelName);
		channel->addClient(client);
	}

	channel->welcomeClient(client);
}