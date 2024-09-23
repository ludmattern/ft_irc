/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCommands.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 18:13:43 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/23 18:37:01 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::setupCommandHandlers()
{
	_commandHandlers["PASS"] = &Server::handlePassCommand;
	_commandHandlers["NICK"] = &Server::handleNickCommand;
	_commandHandlers["USER"] = &Server::handleUserCommand;
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

// Handle NICK command
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
	// Send welcome messages
	sendWelcomeMessages(client);
}