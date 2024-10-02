/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 10:43:51 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/03 01:17:18 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network/Channel.hpp"
#include  "replies.hpp"

Channel::Channel(const std::string& name) : _name(name) {}

void Channel::addClient(Client& client)
{
	_clients[&client] = false;
	client.joinChannel(this);
}

void Channel::addClient(Client& client, bool isOperator)
{
	_clients[&client] = isOperator;
	client.joinChannel(this);
}

void Channel::removeClient(Client& client)
{
	_clients.erase(&client);
	client.partChannel(this);
}

void Channel::setOperator(Client& client, bool isOperator)
{
	_clients[&client] = isOperator;
}

bool Channel::isOperator(Client& client) const
{
	std::map<Client*, bool>::const_iterator it = _clients.find(&client);
	if (it != _clients.end())
		return it->second;
	else
		return false;
}

void Channel::welcomeClient(Client& client)
{
	client.reply(RPL_TOPIC(client.getNickname(), _name, _topic));
	client.reply(RPL_NAMREPLY(client.getNickname(), _name, "liste des clients"));
}

void Channel::broadcast(const std::string& message)
{
	for (std::map<Client*, bool>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
		it->first->write(message);
}

void Channel::broadcast(const std::string& message, Client* sender)
{
	for (std::map<Client*, bool>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->first != sender)
			it->first->write(message);
	}
}

void Channel::addInvite(Client& client)
{
	_invitedClients.insert(&client);
}
