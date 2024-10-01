/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 08:16:09 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/30 18:01:17 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands/Kick.hpp"
#include "Channel.hpp"
#include "Server.hpp"

Kick::Kick() {}

Kick::~Kick() {}

void Kick::execute(Server& server, Client& client, const std::vector<std::string>& params) 
{
	if (params.size() < 2) 
	{
		server.sendError(&client, ERR_NEEDMOREPARAMS, "KICK", "Not enough parameters");
		return;
	}
	std::string channelName = params[0];
	std::string targetNickname = params[1];
	
	std::string reason;
	if (params.size() >= 3)
		reason = params[2];
	else
		reason = "No reason provided";
	Channel* channel = server.getChannelByName(channelName);
	if (!channel)
	{
		server.sendError(&client, ERR_NOSUCHCHANNEL, channelName, "No such channel");
		return;
	}
	if (!channel->hasClient(&client)) 
	{
		server.sendError(&client, ERR_NOTONCHANNEL, channelName, "You're not on that channel");
		return;
	}

	if (!channel->isOperator(&client)) 
	{
		server.sendError(&client, ERR_CHANOPRIVSNEEDED, channelName, "You're not channel operator");
		return;
	}
	Client* targetClient = server.getClientByNickname(targetNickname);
	if (!targetClient) 
	{
		server.sendError(&client, ERR_NOSUCHNICK, targetNickname, "No such nick/channel");
		return;
	}
	if (!channel->hasClient(targetClient)) 
	{
		server.sendError(&client, ERR_USERNOTINCHANNEL, targetNickname, "They aren't on that channel");
		return;
	}

	std::string kickMessage = ":" + client.getNickname() + " KICK " + channelName + " " + targetNickname + " :" + reason;
	channel->broadcastMessage(kickMessage, NULL);
	targetClient->addToOutputBuffer(": KICK " + channelName + " " + targetNickname + " :" + reason + "\n");
	channel->removeClient(targetClient);
}
