/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 11:29:46 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/03 15:07:14 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/Command.hpp"
#include "network/Server.hpp"
#include "network/Client.hpp"
#include "network/Channel.hpp"
#include <sstream>
#include <vector>
#include <algorithm>

Quit::Quit() : Command() {}

Quit::~Quit() {}

void Quit::execute(Client* client, const std::vector<std::string>& params) 
{
	std::string quitMessage = "Client exited";
	if (!params.empty())
	{
		quitMessage = params[0];
	}

	std::string prefix = client->getPrefix();
	std::string message = ":" + prefix + " QUIT";
	if (!quitMessage.empty()) 
	{
		message += " :" + quitMessage;
	}
	message += "\r\n";

	std::set<Channel*> channels = client->getChannels();
	for (std::set<Channel*>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		Channel* channel = *it;
		channel->broadcast(message, client);
		channel->removeClient(client);
	}


	_server.closeClientConnection(client->getFd());
}

