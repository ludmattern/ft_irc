/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 10:44:38 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/28 16:59:18 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include <algorithm>
#include <iostream>

// Constructor
Channel::Channel(Client& client, const std::string& name) : _name(name) 
{
	_operators.insert(&client);
}

Channel::~Channel() {}

// Getters
const std::string& Channel::getName() const {
	return _name;
}

const std::string& Channel::getTopic() const {
	return _topic;
}

// Setters
void Channel::setTopic(const std::string& topic) {
	_topic = topic;
}

// Client management
void Channel::addClient(Client* client) 
{
	_clients.insert(client);
}

void Channel::removeClient(Client* client)
{
	_clients.erase(client);
	if (hasOperator(client))
		_operators.erase(client);
}

bool Channel::hasClient(Client* client) const {
	return _clients.find(client) != _clients.end();
}

const std::set<Client*>& Channel::getClients() const {
	return _clients;
}

void Channel::addOperator(Client* client)
{
    _operators.insert(client);
}

void Channel::removeOperator(Client* client)
{
    _operators.erase(client);
}

bool Channel::hasOperator(Client* client) const 
{
    return _operators.find(client) != _operators.end();
}

const std::set<Client*>& Channel::getOperators() const
{
    return _operators;
}

// mode management
void Channel::addMode(char mode) {
	_modes.insert(mode);
}

void Channel::removeMode(char mode) {
	_modes.erase(mode);
}

bool Channel::hasMode(char mode) const {
	return _modes.find(mode) != _modes.end();
}

// message sending
void Channel::broadcastMessage(const std::string& message, Client* sender) {
	for (std::set<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if (*it != sender)
		{
			(*it)->addToOutputBuffer(message);
		}
	}
}
