/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 11:29:38 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/03 17:25:57 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network/Server.hpp"
#include "network/Client.hpp"
#include "network/Channel.hpp"
#include "commands/Command.hpp"
#include "replies.hpp"
#include <iostream>
Mode::Mode() {}
Mode::~Mode() {}

void Mode::execute(Client* client, const std::vector<std::string>& params)
{
	if (params.size() < 2) 
	{
		client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE"));
		return;
	}

	std::string channelName = params[0];
	std::string modeString = params[1];

	Channel* channel = _server.getChannelByName(channelName);
	if (!channel) 
	{
		client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), channelName));
		return;
	}
	if (!channel->isOperator(client)) 
	{
		client->reply(ERR_CHANOPRIVSNEEDED(client->getNickname(), channelName));
		return;
	}
	char operation = modeString[0];
	if (operation != '-' && operation != '+')
	{
		client->reply(ERR_UMODEUNKNOWNFLAG(client->getNickname()));
		return;
	}
	char mode = modeString[1];
	if (mode != 'i' && mode != 't' && mode != 'k' && mode != 'o' && mode != 'l')
	{
		client->reply(ERR_UMODEUNKNOWNFLAG(client->getNickname()));
		return;
	}
	if (modeString.size() > 2) 
	{
		client->reply(ERR_UMODEUNKNOWNFLAG(client->getNickname()));
		return;
	}
}