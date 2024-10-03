/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 11:29:48 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/03 17:24:36 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/Command.hpp"
#include "network/Server.hpp"
#include "network/Client.hpp"
#include "network/Channel.hpp"
#include "replies.hpp"

Topic::Topic() {}

Topic::~Topic() {}

void Topic::execute(Client* client, const std::vector<std::string>& params)
{
	if (client->getStatus() != REGISTERED)
		client->reply(ERR_NOTREGISTERED());
	else if (params.empty())
	{
		client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), std::string("TOPIC")));
		return;
	}

	std::string channelName = params[0];
	Channel* channel = _server.getChannelByName(channelName);

	if (!channel)
		client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), channelName));
	else if (!channel->hasClient(client))
		client->reply(ERR_NOTONCHANNEL(client->getNickname(), channelName));
	else if (params.size() == 1)
	{
		const std::string& topic = channel->getTopic();
		if (topic.empty())
			client->reply(channelName + " : No topic is set");
		else
			client->reply(RPL_TOPIC(client->getNickname(), channelName, topic));
		return;
	}
	if (!channel->isOperator(client))
	{
		client->reply(ERR_CHANOPRIVSNEEDED(client->getNickname(), channelName));
		return;
	}

	std::string newTopic = params[1];

	for (size_t i = 2; i < params.size(); ++i)
		newTopic += " " + params[i];

	if (!newTopic.empty() && newTopic[0] == ':')
		newTopic = newTopic.substr(1);

	channel->setTopic(newTopic);

	std::string message = ":" + client->getPrefix() + " TOPIC " + channelName + " :" + newTopic + "\r\n";
	channel->broadcast(message);
}