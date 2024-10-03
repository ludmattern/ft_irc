/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 11:29:37 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/03 17:23:13 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network/Server.hpp"
#include "network/Client.hpp"
#include "network/Channel.hpp"
#include "commands/Command.hpp"
#include "replies.hpp"

Kick::Kick() {}
Kick::~Kick() {}

void Kick::execute(Client* client, const std::vector<std::string>& params) 
{
	if (params.size() < 2) 
	{
		client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), std::string("KICK")));
		return;
	}
	std::string channelName = params[0];
	std::string targetNickname = params[1];
	
	std::string reason;
	if (params.size() >= 3)
		reason = params[2];
	else
		reason = "No reason provided";
	Channel* channel = _server.getChannelByName(channelName);
	if (!channel)
	{
		client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), channelName));
		return;
	}
	if (!channel->hasClient(client)) 
	{
		client->reply(ERR_NOTONCHANNEL(client->getNickname(), channelName));
		return;
	}

	if (!channel->isOperator(client)) 
	{
		client->reply(ERR_CHANOPRIVSNEEDED(client->getNickname(), channelName));
		return;
	}
	Client* targetClient = _server.getClientByNickname(targetNickname);
	if (!targetClient) 
	{
		client->reply(ERR_NOSUCHNICK(targetNickname));
		return;
	}
	if (!channel->hasClient(targetClient)) 
	{
		client->reply(ERR_USERNOTINCHANNEL(client->getNickname(), targetClient->getNickname(), channel->getName()));
		return;
	}

	std::string kickMessage = ":" + client->getNickname() + " KICK " + channelName + " " + targetNickname + " :" + reason;
	channel->broadcast(kickMessage);
	targetClient->write(": KICK " + channelName + " " + targetNickname + " :" + reason);
	channel->removeClient(targetClient);
}