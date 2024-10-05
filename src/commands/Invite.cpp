/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 11:29:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/05 16:44:24 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network/Server.hpp"
#include "network/Client.hpp"
#include "network/Channel.hpp"
#include "commands/Command.hpp"
#include "replies.hpp"

Invite::Invite() {}
Invite::~Invite() {}

void Invite::execute(Client* client, const std::vector<std::string>& params)
{
	if (client->getStatus() != REGISTERED)
	{
		client->reply(ERR_NOTREGISTERED());
		return;
	}
	if (params.size() < 2)
	{
		client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), std::string("INVITE")));
		return;
	}
	std::string targetNickname = params[0];
	std::string channelName = params[1];

	Channel* channel = _server.getChannel(channelName);
	if (!channel->isOperator(client))
	{
		client->reply(ERR_CHANOPRIVSNEEDED(client->getNickname(), channelName));
		return;
	}
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
	Client* targetClient = Server::getInstance().getClientByNickname(targetNickname);
	if (!targetClient)
	{
		client->reply(ERR_NOSUCHNICK(targetNickname));
		return;
	}
	if (channel->hasClient(targetClient))
	{
		client->reply(ERR_USERONCHANNEL(client->getNickname(), targetNickname, channelName));
		return;
	}
	channel->addInvite(*targetClient);

	std::string inviteMessage = ":" + client->getPrefix() + " INVITE " + targetNickname + " :" + channelName;
	targetClient->write(inviteMessage);

	client->reply(RPL_INVITING(targetNickname, channelName));
}
