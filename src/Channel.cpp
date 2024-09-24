/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 10:44:38 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/24 16:57:53 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include <algorithm>

// Constructor
Channel::Channel(const std::string& name) : _name(name) {}

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
void Channel::addClient(Client* client) {
	_clients.insert(client);
}

void Channel::removeClient(Client* client) {
	_clients.erase(client);
}

bool Channel::hasClient(Client* client) const {
	return _clients.find(client) != _clients.end();
}

const std::set<Client*>& Channel::getClients() const {
	return _clients;
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
