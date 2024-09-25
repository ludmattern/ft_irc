/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCommands.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 18:13:43 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/25 10:36:10 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include<algorithm>

void Server::setupCommandHandlers() {
	_commandHandlers["PASS"] = &Server::handlePassCommand;
	_commandHandlers["NICK"] = &Server::handleNickCommand;
	_commandHandlers["USER"] = &Server::handleUserCommand;
	_commandHandlers["JOIN"] = &Server::handleJoinCommand;
	_commandHandlers["PART"] = &Server::handlePartCommand;
	_commandHandlers["PRIVMSG"] = &Server::handlePrivmsgCommand;
	_commandHandlers["NOTICE"] = &Server::handleNoticeCommand;
	_commandHandlers["QUIT"] = &Server::handleQuitCommand;
	_commandHandlers["TOPIC"] = &Server::handleTopicCommand;
	_commandHandlers["PING"] = &Server::handlePingCommand;
	_commandHandlers["PONG"] = &Server::handlePongCommand;
	_commandHandlers["KICK"] = &Server::handleKickCommand;
	_commandHandlers["INVITE"] = &Server::handleInviteCommand;
}

void Server::processClientCommand(Client* client, const std::string& commandLine) 
{
	std::string line = commandLine;
	std::string prefix;


	if (!line.empty() && line[0] == ':') 
	{
		size_t pos = line.find(' ');
		if (pos != std::string::npos) 
		{
			prefix = line.substr(1, pos - 1);
			line = line.substr(pos + 1);
		} 
		else 
		{
			sendError(client, ERR_UNKNOWNCOMMAND, "", "Malformed command");
			return;
		}
	}
	
	std::istringstream iss(line);
	std::string command;
	iss >> command;

	std::vector<std::string> params;
	std::string param;
	while (iss >> param) 
	{
		if (param[0] == ':') 
		{
			std::string rest;
			getline(iss, rest);
			if (!rest.empty())
				param += rest;
			param = param.substr(1);
			params.push_back(param);
			break;
		} 
		else 
			params.push_back(param);
	}

	std::transform(command.begin(), command.end(), command.begin(), ::toupper);

	std::map<std::string, CommandHandler>::iterator it = _commandHandlers.find(command);
	if (it != _commandHandlers.end()) 
	{
		CommandHandler handler = it->second;
		(this->*handler)(client, params);
	}
	else 
		sendError(client, ERR_UNKNOWNCOMMAND, command, "Unknown command");
}



void Server::handlePassCommand(Client* client, const std::vector<std::string>& params) 
{
	if (client->isRegistered())
		sendError(client, ERR_ALREADYREGISTRED, "", "You may not reregister");
	else if (params.empty())
		sendError(client, ERR_NEEDMOREPARAMS, "PASS", "Not enough parameters");
	else
		client->setPassword(params[0]);
}


void Server::handleNickCommand(Client* client, const std::vector<std::string>& params)
{
	if (params.empty())
	{
		sendError(client, ERR_NONICKNAMEGIVEN, "", "No nickname given");
		return;
	}

	std::string nickname = params[0];

	if (isNicknameTaken(nickname))
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


void Server::handleUserCommand(Client* client, const std::vector<std::string>& params)
{
	if (client->isRegistered())
	{
		sendError(client, ERR_ALREADYREGISTRED, "", "You may not reregister");
		return;
	}

	if (params.size() < 4)
	{
		sendError(client, ERR_NEEDMOREPARAMS, "USER", "Not enough parameters");
		return;
	}

	std::string username = params[0];
	std::string hostname = params[1];
	std::string servername = params[2];
	std::string realname = params[3];

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

void Server::handleJoinCommand(Client* client, const std::vector<std::string>& params)
{
	if (!validateJoinCommand(client, params))
		return;

	std::string channelName = params[0];

	Channel* channel = getOrCreateChannel(channelName);

	addClientToChannel(channel, client);
	sendChannelInfoToClient(channel, client);
}

bool Server::validateJoinCommand(Client* client, const std::vector<std::string>& params)
{
	if (!client->isRegistered())
	{
		sendError(client, ERR_NOTREGISTERED, "JOIN", "You are not registered");
		return false;
	}

	if (params.empty())
	{
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
	sendRawMessageToClient(client, joinMessage);
	channel->broadcastMessage(joinMessage, client);
}


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
void Server::handlePrivmsgCommand(Client* client, const std::vector<std::string>& params)
{
	if (!client->isRegistered())
	{
		sendError(client, ERR_NOTREGISTERED, "PRIVMSG", "You are not registered");
		return;
	}

	if (params.size() < 2)
	{
		sendError(client, ERR_NEEDMOREPARAMS, "PRIVMSG", "Not enough parameters");
		return;
	}

	std::string target = params[0];
	std::string message = params[1];

	if (message[0] == ':')
		message.erase(0, 1); // Supprimer le ':' initial

	if (target[0] == GLOBAL_CHANNEL || target[0] == LOCAL_CHANNEL)
	{
		std::map<std::string, Channel*>::iterator it = _channels.find(target);
		if (it == _channels.end())
		{
			sendError(client, ERR_NOSUCHCHANNEL, target, "No such channel");
			return;
		}

		Channel* channel = it->second;
		if (!channel->hasClient(client)) {
			sendError(client, ERR_CANNOTSENDTOCHAN, target, "Cannot send to channel");
			return;
		}

		std::string fullMessage = ":" + client->getPrefix() + " PRIVMSG " + target + " :" + message + "\r\n";
		broadcastToChannel(channel, fullMessage, client);
	}
	else
	{
		Client* recipient = NULL;
		for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		{
			if (it->second->getNickname() == target)
			{
				recipient = it->second;
				break;
			}
		}
		if (recipient == NULL)
		{
			sendError(client, ERR_NOSUCHNICK, target, "No such nick/channel");
			return;
		}

		std::string fullMessage = ":" + client->getPrefix() + " PRIVMSG " + target + " :" + message + "\r\n";
		sendRawMessageToClient(recipient, fullMessage);
	}
}


void Server::handlePartCommand(Client* client, const  std::vector<std::string>& params) {
	(void)client;
	(void)params;

	logToServer("PART command received: ", "INFO");
}


void Server::handleNoticeCommand(Client* client, const  std::vector<std::string>& params) {
	(void)client;
	(void)params;

	logToServer("NOTICE command received: ", "INFO");
}

void Server::handleQuitCommand(Client* client, const  std::vector<std::string>& params) {
	(void)client;
	(void)params;

	logToServer("QUIT command received: ", "INFO");
}

void Server::handleTopicCommand(Client* client, const  std::vector<std::string>& params) {
	(void)client;
	(void)params;

	logToServer("TOPIC command received: ", "INFO");
}

void Server::handlePingCommand(Client* client, const  std::vector<std::string>& params) {
	(void)client;
	(void)params;

	logToServer("PING command received: ", "INFO");
}

void Server::handlePongCommand(Client* client, const  std::vector<std::string>& params) {
	(void)client;
	(void)params;
	logToServer("PONG command received: ", "INFO");
}

void Server::handleKickCommand(Client* client, const  std::vector<std::string>& params) {
	(void)client;
	(void)params;

	logToServer("KICK command received: ", "INFO");
}

void Server::handleInviteCommand(Client* client, const  std::vector<std::string>& params) {
	(void)client;
	(void)params;
	logToServer("INVITE command received: ", "INFO");
}