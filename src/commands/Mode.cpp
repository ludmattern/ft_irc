/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 11:29:38 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/05 16:44:24 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network/Server.hpp"
#include "network/Client.hpp"
#include "network/Channel.hpp"
#include "commands/Command.hpp"
#include "libs/cppLibft.hpp"
#include "replies.hpp"
#include <iostream>
#include <string>
#include <sstream>

Mode::Mode() {}
Mode::~Mode() {}

void Mode::execute(Client* client, const std::vector<std::string>& params)
{
	if (params.size() == 0) 
	{
		client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE"));
		return;
	}
	std::string channelName = params[0];
	Channel* channel = _server.getChannel(channelName);
	if (!channel) 
	{
		client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), channelName));
		return;
	}
	if (params.size() == 1)
	{
		std::string modes = channel->getModes();
		modes = '+' + modes;
		client->reply(RPL_CHANNELMODEIS(client->getNickname(), channel->getName(), modes));
		return ;
	}
	std::string modeString = params[1];

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
	if (modeString.size() != 2) 
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

	if (operation == '+')
	{
		if (mode == 'i' || mode == 't')
			channel->setMode(mode);
		else if (mode == 'k')
		{
			if (params.size() < 3)
			{
				client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE"));
				return;
			}
			std::string key = params[2];
			channel->setPassword(key);
			channel->setMode('k');
		}
		else if (mode == 'l')
		{
			if (params.size() < 3)
			{
				client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE"));
				return;
			}
			std::stringstream ss(params[2]);
			int limit;
			if (!(ss >> limit)) 
			{
				client->reply("Invalid limit value.");
				return;
			}
			if (limit < channel->getNumberOfClients())
			{
				client->reply(ERR_INVALIDMODEPARAM(client->getNickname(), channelName, " Error : Cannot set limit lower than current number of users"));
				return;
			}
			channel->setLimit(limit);
			channel->setMode('l');
		}
		else if (mode == 'o')
		{
			if (params.size() < 3)
			{
				client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE"));
				return;
			}
			std::string targetNickname = params[2];
			Client* targetClient = _server.getClientByNickname(targetNickname);
			if (!targetClient)
			{
				client->reply(ERR_NOSUCHNICK(targetNickname));
				return;
			}
			if (!channel->hasClient(targetClient))
			{
				client->reply(ERR_USERNOTINCHANNEL(client->getNickname(), targetNickname, channelName));
				return;
			}
			channel->setOperator(targetClient, true);
		}
	}
	else if (operation == '-')
	{
		if (mode == 'i' || mode == 't')
			channel->removeMode(mode);
		else if (mode == 'k')
		{
			channel->setPassword("");
			channel->removeMode('k');
		}
		else if (mode == 'l')
		{
			channel->setLimit(-1);
			channel->removeMode('l');
		}
		else if (mode == 'o')
		{
			if (params.size() < 3)
			{
				client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE"));
				return;
			}
			std::string targetNickname = params[2];
			Client* targetClient = _server.getClientByNickname(targetNickname);
			if (!targetClient)
			{
				client->reply(ERR_NOSUCHNICK(targetNickname));
				return;
			}
			if (!channel->hasClient(targetClient))
			{
				client->reply(ERR_USERNOTINCHANNEL(client->getNickname(), targetNickname, channelName));
				return;
			}
			channel->setOperator(targetClient, false);
		}
	}
	std::string modeChangeMessage = ":" + client->getPrefix() + " MODE " + channelName + " " + operation + mode;
	if ((mode == 'k' || mode == 'l' || mode == 'o') && params.size() >= 3)
		modeChangeMessage += " " + params[2];
	channel->broadcast(modeChangeMessage);
}
