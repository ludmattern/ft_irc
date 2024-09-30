/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 14:29:21 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/30 10:26:18 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands/Part.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include <sstream>
#include <vector>
#include <algorithm>

Part::Part() {}

Part::~Part() {}

void Part::execute(Server& server, Client& client, const std::vector<std::string>& params) {
	if (!client.isRegistered()) {
		server.sendError(&client, ERR_NOTREGISTERED, "PART", "You have not registered");
		return;
	}

	if (params.empty()) {
		server.sendError(&client, ERR_NEEDMOREPARAMS, "PART", "Not enough parameters");
		return;
	}

	// Parse channels
	std::string channelsParam = params[0];
	std::stringstream ss(channelsParam);
	std::string channelName;

	// Optional part message
	std::string partMessage = (params.size() > 1) ? params[1] : "";

	while (std::getline(ss, channelName, ',')) {
		Channel* channel = server.getChannelByName(channelName);

		if (!channel) {
			server.sendError(&client, ERR_NOSUCHCHANNEL, channelName, "No such channel");
			continue;
		}

		if (!channel->hasClient(&client)) {
			server.sendError(&client, ERR_NOTONCHANNEL, channelName, "You're not on that channel");
			continue;
		}

		// Prepare the PART message before updating state
		std::string prefix = client.getPrefix();
		std::string message = ":" + prefix + " PART " + channelName;
		if (!partMessage.empty()) {
			message += " :" + partMessage;
		}
		message += CRLF;

		// Notify all clients in the channel, including the sender
		server.broadcastToChannel(channel, message, NULL); // Include all clients

		// Now update server and client state
		channel->removeClient(&client);
		client.leaveChannel(channelName);

		// If the channel is empty, delete it
		if (channel->getClients().empty()) {
			server.removeChannel(channelName);
			delete channel;
		}
	}
}
