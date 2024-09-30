/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 08:17:38 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/30 14:07:27 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands/Topic.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include <sstream>
#include <vector>
#include <algorithm>

Topic::Topic() {}

Topic::~Topic() {}

void Topic::execute(Server& server, Client& client, const std::vector<std::string>& params) {
	if (!client.isRegistered())
		server.sendError(&client, ERR_NOTREGISTERED, "TOPIC", "You have not registered");
	else if (params.empty())
	{
		server.sendError(&client, ERR_NEEDMOREPARAMS, "TOPIC", "Not enough parameters");
		return;
	}

	std::string channelName = params[0];
	Channel* channel = server.getChannelByName(channelName);

	if (!channel)
		server.sendError(&client, ERR_NOSUCHCHANNEL, channelName, "No such channel");
	else if (!channel->hasClient(&client))
		server.sendError(&client, ERR_NOTONCHANNEL, channelName, "You're not on that channel");
	else if (params.size() == 1)
	{
		// Pas de nouveau sujet fourni, on renvoie le sujet actuel
		const std::string& topic = channel->getTopic();
		if (topic.empty())
			server.sendReply(&client, RPL_NOTOPIC, channelName, "No topic is set");
		else
			server.sendReply(&client, RPL_TOPIC, channelName + " :" + topic, "");
		return;
	}
	// Vérifier les permissions pour changer le sujet
	if (channel->hasMode('t') && !channel->isOperator(&client))
	{
		server.sendError(&client, ERR_CHANOPRIVSNEEDED, channelName, "You're not channel operator");
		return;
	}

	std::string newTopic = params[1];

	for (size_t i = 2; i < params.size(); ++i)
		newTopic += " " + params[i];

	if (!newTopic.empty() && newTopic[0] == ':')
		newTopic = newTopic.substr(1);

	channel->setTopic(newTopic);

	// Informer tous les membres du canal du nouveau sujet
	std::string message = ":" + client.getPrefix() + " TOPIC " + channelName + " :" + newTopic + "\r\n";
	server.broadcastToChannel(channel, message, NULL);
}
