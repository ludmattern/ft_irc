/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 10:43:51 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/07 11:31:45 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network/Channel.hpp"
#include  "replies.hpp"

Channel::Channel(const std::string& name) : _limit(5), _name(name), _password("") {}

void Channel::addClient(Client* client)
{
	_clients[client] = false;
}

void Channel::addClient(Client* client, bool isOperator)
{
	_clients[client] = isOperator;
}

void Channel::removeClient(Client* client)
{
	_clients.erase(client);
	client->partChannel(this);
}

void Channel::setOperator(Client* client, bool isOperator)
{
	_clients[client] = isOperator;
}

bool Channel::isOperator(Client* client) const
{
	std::map<Client*, bool>::const_iterator it = _clients.find(client);
	if (it != _clients.end())
		return it->second;
	else
		return false;
}

void Channel::welcomeClient(Client* client)
{
	client->reply(RPL_TOPIC(client->getNickname(), _name, _topic));
	client->reply(RPL_NAMREPLY(client->getNickname(), _name, getClients()));
	client->reply(RPL_ENDOFNAMES(client->getNickname(), _name));
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

std::string Channel::getClients() const
{
    std::string userList;
    for (std::map<Client*, bool>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->second)
            userList += "@";
        userList += it->first->getNickname() + " ";
    }
    return userList;
}

std::string Channel::getModes() const
{
	std::string modes;
	for (std::set<char>::const_iterator it = _modes.begin(); it != _modes.end(); ++it)
		modes += *it;
	return modes;
}

std::map<Client*, bool> Channel::getClientsmap() const
{
	return _clients;
}