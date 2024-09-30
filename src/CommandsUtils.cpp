/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandsUtils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 14:53:55 by fprevot           #+#    #+#             */
/*   Updated: 2024/09/30 10:25:34 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::sendChannelInfoToClient(Channel* channel, Client* client)
{
	// Send the topic if it exists
	if (!channel->getTopic().empty())
		sendReply(client, RPL_TOPIC, channel->getName(), channel->getTopic());
	else
		sendReply(client, RPL_NOTOPIC, channel->getName(), "No topic is set");

	// Build the names list
	std::string namesList;
	for (std::set<Client*>::const_iterator it = channel->getClients().begin(); it != channel->getClients().end(); ++it)
		namesList += (*it)->getNickname() + " ";

	// Remove trailing space
	if (!namesList.empty())
		namesList.erase(namesList.size() - 1);

	// :<server> 353 <nick> <symbol> <channel> :<names list>
	std::string symbol = "="; // "=" denotes a public channel
	std::string params = symbol + " " + channel->getName();
	sendReply(client, RPL_NAMREPLY, params, namesList);

	sendReply(client, RPL_ENDOFNAMES, channel->getName(), "End of /NAMES list");

	logToServer("Client " + client->getNickname() + " joined channel " + channel->getName(), "INFO");
}

Channel* Server::getOrCreateChannel(Client& client, const std::string& channelName)
{
	std::map<std::string, Channel*>::iterator it = _channels.find(channelName);
	if (it == _channels.end()) {
		Channel* newChannel = new Channel(client, channelName);
		_channels[channelName] = newChannel;
		return newChannel;
	}
	return it->second;
}

void Server::addClientToChannel(Channel* channel, Client* client)
{
	channel->addClient(client);
	client->joinChannel(channel->getName());

	std::string joinMessage = ":" + client->getPrefix() + " JOIN :" + channel->getName() + CRLF;
	sendRawMessageToClient(client, joinMessage);
	channel->broadcastMessage(joinMessage, client);
}


bool Server::isNicknameTaken(const std::string& nickname) const
{
	for (std::map<int, Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->second->getNickname() == nickname)
			return true;
	}
	return false;
}

void Server::registerClientIfReady(Client* client)
{
	if (client->getNickname().empty() || client->getUsername().empty())
		return;

	if (!_password.empty())
	{
		if (client->getPassword() != _password)
		{
			sendError(client, ERR_PASSWDMISMATCH, "", "Password incorrect");
			return ;
		}
	}

	client->setRegistered(true);
	sendWelcomeMessages(client);
}