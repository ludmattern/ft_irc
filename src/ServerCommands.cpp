/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCommands.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 18:13:43 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/24 11:21:28 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::setupCommandHandlers()
{
	_commandHandlers["PASS"] = &Server::handlePassCommand;
	_commandHandlers["NICK"] = &Server::handleNickCommand;
	_commandHandlers["USER"] = &Server::handleUserCommand;
	_commandHandlers["JOIN"] = &Server::handleJoinCommand;
}

void Server::processClientCommand(Client* client, const std::string& commandLine)
{
	std::istringstream iss(commandLine);
	std::string command;
	iss >> command;

	std::string params;
	getline(iss, params);
	if (!params.empty() && params[0] == ' ')
		params.erase(0, 1); // Remove leading space

	std::map<std::string, CommandHandler>::iterator it = _commandHandlers.find(command);
	if (it != _commandHandlers.end())
	{
		CommandHandler handler = it->second;
		(this->*handler)(client, params);
	}
	else
		sendError(client, ERR_UNKNOWNCOMMAND, command, "Unknown command");
}

void Server::handlePassCommand(Client* client, const std::string& params)
{
	if (client->isRegistered())
		sendError(client, ERR_ALREADYREGISTRED, "", "You may not reregister");
	else if (params.empty())
		sendError(client, ERR_NEEDMOREPARAMS, "PASS", "Not enough parameters");
	else
		client->setPassword(params);
}

void Server::handleNickCommand(Client* client, const std::string& nickname)
{
	if (nickname.empty())
		sendError(client, ERR_NONICKNAMEGIVEN, "", "No nickname given");
	else if (isNicknameTaken(nickname))
		sendError(client, ERR_NICKNAMEINUSE, nickname, "Nickname is already in use");
	else
	{
		client->setNickname(nickname);
		registerClientIfReady(client);
	}
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


void Server::handleUserCommand(Client* client, const std::string& params)
{
	if (client->isRegistered())
	{
		sendError(client, ERR_ALREADYREGISTRED, "", "You may not reregister");
		return;
	}

	std::istringstream iss(params);
	std::string username, hostname, servername, realname;

	iss >> username >> hostname >> servername;
	std::getline(iss, realname);

	if (username.empty() || hostname.empty() || servername.empty() || realname.empty())
	{
		sendError(client, ERR_NEEDMOREPARAMS, "USER", "Not enough parameters");
		return;
	}

	if (realname[0] == ':')
	{
		realname.erase(0, 1);
	}

	client->setUsername(username);
	client->setRealname(realname);

	registerClientIfReady(client);
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
			client->markForDisconnection(true);
			return ;
		}
	}

	client->setRegistered(true);
	sendWelcomeMessages(client);
}

void Server::handleJoinCommand(Client* client, const std::string& params)
{
	if (!validateJoinCommand(client, params)) {
		return;
	}

	std::string channelName;
	std::istringstream iss(params);
	iss >> channelName;

	Channel* channel = getOrCreateChannel(channelName);

	addClientToChannel(channel, client);
	sendChannelInfoToClient(channel, client);
}

bool Server::validateJoinCommand(Client* client, const std::string& params)
{
    if (!client->isRegistered()) {
        sendError(client, "451", "JOIN", "You are not registered");
        return false;
    }

    if (params.empty()) {
        sendError(client, ERR_NEEDMOREPARAMS, "JOIN", "Not enough parameters");
        return false;
    }

    return true;
}

Channel* Server::getOrCreateChannel(const std::string& channelName)
{
    std::map<std::string, Channel*>::iterator it = _channels.find(channelName);
    if (it == _channels.end()) {
        Channel* newChannel = new Channel(channelName);
        _channels[channelName] = newChannel;
        return newChannel;
    }
    return it->second;
}

void Server::addClientToChannel(Channel* channel, Client* client)
{
    channel->addClient(client);
    client->joinChannel(channel->getName());

    std::string joinMessage = ":" + client->getPrefix() + " JOIN :" + channel->getName() + "\r\n";
    channel->broadcastMessage(joinMessage, client);
}

void Server::sendChannelInfoToClient(Channel* channel, Client* client)
{
    if (!channel->getTopic().empty()) {
        sendReply(client, "332", channel->getName(), channel->getTopic());
    }

    std::string namesList = "=" + channel->getName() + " :";
    for (std::set<Client*>::const_iterator it = channel->getClients().begin(); it != channel->getClients().end(); ++it) {
        namesList += (*it)->getNickname() + " ";
    }

    sendReply(client, "353", namesList, "");
    sendReply(client, "366", channel->getName(), "End of /NAMES list");
	logToServer("Client " + client->getNickname() + " joined channel " + channel->getName());
}
