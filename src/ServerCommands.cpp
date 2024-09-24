/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCommands.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 18:13:43 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/24 15:35:43 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::setupCommandHandlers()
{
	_commandHandlers["PASS"] = &Server::handlePassCommand;
	_commandHandlers["NICK"] = &Server::handleNickCommand;
	_commandHandlers["USER"] = &Server::handleUserCommand;
	_commandHandlers["JOIN"] = &Server::handleJoinCommand;
	_commandHandlers["PRIVMSG"] = &Server::handlePrivmsgCommand;
	_commandHandlers["PART"] = &Server::handlePartCommand;
	_commandHandlers["NOTICE"] = &Server::handleNoticeCommand;
	_commandHandlers["QUIT"] = &Server::handleQuitCommand;
	_commandHandlers["TOPIC"] = &Server::handleTopicCommand;
	_commandHandlers["PING"] = &Server::handlePingCommand;
	_commandHandlers["PONG"] = &Server::handlePongCommand;
	_commandHandlers["KICK"] = &Server::handleKickCommand;
	_commandHandlers["INVITE"] = &Server::handleInviteCommand;
	_commandHandlers["TOPIC"] = &Server::handleTopicCommand;
}

void Server::processClientCommand(Client* client, const std::string& commandLine)
{
	std::istringstream iss(commandLine);
	std::string command;
	iss >> command;

	std::string params;
	getline(iss, params);
	if (!params.empty() && params[0] == ' ')
		params.erase(0, 1);

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
	if (!validateJoinCommand(client, params))
		return;

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

    // Construct the JOIN message
    std::string joinMessage = ":" + client->getPrefix() + " JOIN :" + channel->getName() + "\r\n";

    // Send the JOIN message to the client
    sendRawMessageToClient(client, joinMessage);

    // Broadcast the JOIN message to other clients in the channel
    channel->broadcastMessage(joinMessage, client);
}


void Server::sendChannelInfoToClient(Channel* channel, Client* client)
{
	// Send the topic if it exists
	if (!channel->getTopic().empty())
		sendReply(client, RPL_TOPIC, channel->getName(), channel->getTopic());
	else
		sendReply(client, "331", channel->getName(), "No topic is set");

	// Build the names list
	std::string namesList;
	for (std::set<Client*>::const_iterator it = channel->getClients().begin(); it != channel->getClients().end(); ++it) {
		namesList += (*it)->getNickname() + " ";
	}

	// Remove trailing space
	if (!namesList.empty()) {
		namesList.erase(namesList.size() - 1);
	}

	// Correctly format the parameters for RPL_NAMREPLY
	// :<server> 353 <nick> <symbol> <channel> :<names list>
	std::string symbol = "="; // "=" denotes a public channel
	std::string params = symbol + " " + channel->getName();

	// Send RPL_NAMREPLY
	sendReply(client, RPL_NAMREPLY, params, namesList);

	// Send RPL_ENDOFNAMES
	sendReply(client, RPL_ENDOFNAMES, channel->getName(), "End of /NAMES list");

	logToServer("Client " + client->getNickname() + " joined channel " + channel->getName());
}

void Server::handlePrivmsgCommand(Client* client, const std::string& params) {
    if (!client->isRegistered()) {
        sendError(client, "451", "PRIVMSG", "You are not registered");
        return;
    }

    std::istringstream iss(params);
    std::string target;
    iss >> target;

    std::string message;
    getline(iss, message);

    if (target.empty() || message.empty()) {
        sendError(client, ERR_NEEDMOREPARAMS, "PRIVMSG", "Not enough parameters");
        return;
    }

    if (message[0] == ':')
        message.erase(0, 1); // Supprimer le ':' initial

    // Vérifier si la cible est un canal
    if (target[0] == '#' || target[0] == '&') {
        std::map<std::string, Channel*>::iterator it = _channels.find(target);
        if (it == _channels.end()) {
            sendError(client, "403", target, "No such channel");
            return;
        }

        Channel* channel = it->second;
        if (!channel->hasClient(client)) {
            sendError(client, "404", target, "Cannot send to channel");
            return;
        }

        // Envoyer le message à tous les membres du canal
        std::string fullMessage = ":" + client->getPrefix() + " PRIVMSG " + target + " :" + message + "\r\n";
        broadcastToChannel(channel, fullMessage, client);
    } else {
        // Gérer les messages privés à un utilisateur
        Client* recipient = NULL;
        for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
            if (it->second->getNickname() == target) {
                recipient = it->second;
                break;
            }
        }
        if (recipient == NULL) {
            sendError(client, "401", target, "No such nick/channel");
            return;
        }

        // Envoyer le message au destinataire
        std::string fullMessage = ":" + client->getPrefix() + " PRIVMSG " + target + " :" + message + "\r\n";
        sendRawMessageToClient(recipient, fullMessage);
    }
}


void Server::handlePartCommand(Client* client, const std::string& params) {
	(void)client;
    std::cout << "PART command received: " << params << std::endl;
}


void Server::handleNoticeCommand(Client* client, const std::string& params) {
	(void)client;
    std::cout << "NOTICE command received: " << params << std::endl;
}

void Server::handleQuitCommand(Client* client, const std::string& params) {
	(void)client;
    std::cout << "QUIT command received: " << params << std::endl;
}

void Server::handleTopicCommand(Client* client, const std::string& params) {
	(void)client;
    std::cout << "TOPIC command received: " << params << std::endl;
}

void Server::handlePingCommand(Client* client, const std::string& params) {
	(void)client;
    std::cout << "PING command received: " << params << std::endl;
}

void Server::handlePongCommand(Client* client, const std::string& params) {
	(void)client;
    std::cout << "PONG command received: " << params << std::endl;
}

void Server::handleKickCommand(Client* client, const std::string& params) {
	(void)client;
    std::cout << "KICK command received: " << params << std::endl;
}

void Server::handleInviteCommand(Client* client, const std::string& params) {
	(void)client;
    std::cout << "INVITE command received: " << params << std::endl;
}