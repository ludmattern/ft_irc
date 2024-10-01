/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 12:56:08 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/01 16:47:51 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network/Client.hpp"
#include "replies.hpp"
#include <stdexcept>
#include <sys/socket.h>

// Constructor and destructor
Client::Client(int fd, const std::string &hostname)
	: _fd(fd), _hostname(hostname), _status(HANDSHAKE) {}
Client::~Client() {}

// Getters
int Client::getFd() const { return _fd; }
std::string Client::getNickname() const { return _nickname; }
std::string Client::getUsername() const { return _username; }
std::string Client::getRealname() const { return _realname; }
std::string Client::getHostname() const { return _hostname; }
ClientStatus Client::getStatus() const { return _status; }
std::string Client::getPrefix() const 
{
	std::string username = _username.empty() ? "" : "!" + _username;
	std::string hostname = "@" + _hostname;

	return (_nickname + username + hostname);
}

void Client::write(const std::string& message) const
{
	std::string buffer = message + CRLF;
	if (send(_fd, buffer.c_str(), buffer.length(), 0) < 0)
		throw std::runtime_error("Error while writing to client");
}

void Client::reply(const std::string& reply)
{
	this->write(":" + getPrefix() + " " + reply);
}

void Client::joinChannel(Channel* channel/*, bool isOperator*/)
{
	(void)isOperator;
	_channels.insert(channel);
	//channel->addClient(this, isOperator);
}

const std::set<Channel*>& Client::getChannels() const
{
	return _channels;
}

void Client::partChannel(Channel* channel)
{
	_channels.erase(channel);
	channel->removeClient(this);
}

void Client::setNickname(const std::string& nickname) {	_nickname = nickname; }
void Client::setUsername(const std::string& username) { _username = username; }
void Client::setRealname(const std::string& realname) {	_realname = realname; }
void Client::setStatus(ClientStatus status) { _status = status; }