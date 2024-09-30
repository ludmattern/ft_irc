/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 08:16:03 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/30 08:24:01 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands/Join.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

Join::Join() {}
Join::~Join() {}

void Join::execute(Server& server, Client& client, const std::vector<std::string>& params)
{
	if (!client.isRegistered())
	{
		server.sendError(&client, ERR_NOTREGISTERED, "JOIN", "You are not registered");
		return;
	}

	if (params.empty())
	{
		server.sendError(&client, ERR_NEEDMOREPARAMS, "JOIN", "Not enough parameters");
		return;
	}

	std::string channelName = params[0];

	Channel* channel = server.getOrCreateChannel(client ,channelName);

	server.addClientToChannel(channel, &client);
	server.sendChannelInfoToClient(channel, &client);
}
