/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 11:29:38 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/03 18:39:56 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network/Server.hpp"
#include "network/Client.hpp"
#include "network/Channel.hpp"
#include "commands/Command.hpp"
#include "replies.hpp"
#include <iostream>
#include <string>
Mode::Mode() {}
Mode::~Mode() {}
#include <sstream>

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
        {
            channel->setMode(mode);
        }
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
			if (!(ss >> limit)) {
				client->reply("Invalid limit value.");
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
        {
            channel->removeMode(mode);
        }
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
    modeChangeMessage += "\r\n";
    channel->broadcast(modeChangeMessage);
}
