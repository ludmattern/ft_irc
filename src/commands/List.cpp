/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   List.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 16:39:43 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/05 18:03:50 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/Command.hpp"
#include "network/Server.hpp"
#include "network/Client.hpp"
#include "libs/cppLibft.hpp"

#include "replies.hpp"

List::List() : Command() {}
List::~List() {}

void List::execute(Client* client, const std::vector<std::string>& params)
{
	(void)params;
	if (client->getStatus() != REGISTERED)
	{
		client->reply(ERR_NOTREGISTERED());
		return;
	}

	client->reply(RPL_LISTSTART(client->getNickname()));

	std::map<std::string, Channel*> channels = _server.getChannels();
	for (std::map<std::string, Channel*>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		Channel* channel = it->second;
		std::string topic = channel->getTopic();
		if (topic.empty())
			topic = "No topic is set";

		client->reply(RPL_LIST(client->getNickname(), channel->getName(), toString(channel->getNumberOfClients()), topic));
	}
	client->reply(RPL_LISTEND(client->getNickname()));
}
