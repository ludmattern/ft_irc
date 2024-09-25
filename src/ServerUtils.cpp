/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 18:15:29 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/25 10:07:44 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <sstream>


void Server::sendWelcomeMessages(Client* client)
{
	std::string prefix = client->getPrefix();
	sendReply(client, RPL_WELCOME, "", "Welcome to the IRC server " + prefix);
	sendReply(client, RPL_YOURHOST, "", "Your host is " + _serverName + ", running version 1.0");
	sendReply(client, RPL_CREATED, "", "This server was created today");
	sendReply(client, RPL_MYINFO, _serverName + " 1.0", "Available user modes: ...");
}

// Send standard reply to client
void Server::sendReply(Client* client, const std::string& code, const std::string& params, const std::string& message)
{
	std::string nickname = client->getNickname();
	if (nickname.empty())
		nickname = "*";
	std::string reply = ":" + _serverName + " " + code + " " + nickname;
	if (!params.empty())
		reply += " " + params;
	if (!message.empty())
		reply += " :" + message;
	reply += "\r\n";
	sendRawMessageToClient(client, reply);
}

// Send error to client
void Server::sendError(Client* client, const std::string& code, const std::string& command, const std::string& message)
{
	std::string nickname = client->getNickname();
	if (nickname.empty())
		nickname = "*";
	std::string error = ":" + _serverName + " " + code + " " + nickname;
	if (!command.empty())
		error += " " + command;
	error += " :" + message + "\r\n";
	sendRawMessageToClient(client, error);
}

#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <algorithm>

#define RESET "\033[0m"
#define BOLD "\033[1m"

void Server::logToServer(const std::string& message, const std::string& level) {
    if (message.empty())
        return;

    std::string color;
    if (level == "INFO")
        color = GREEN;
    else if (level == "WARNING")
        color = YELLOW;
    else if (level == "ERROR")
        color = RED;
    else if (level == "DEBUG")
        color = CYAN;
    else
        color = RESET; // Par défaut

    std::string formattedMessage = message;

    // Parcourir les channels et utilisateurs pour les mettre en gras
    for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
        std::string channelName = it->first;
        size_t pos = formattedMessage.find(channelName);
        if (pos != std::string::npos) {
            formattedMessage.replace(pos, channelName.length(), BOLD + channelName + RESET);
        }
    }

    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        std::string clientName = it->second->getNickname();
        size_t pos = formattedMessage.find(clientName);
        if (pos != std::string::npos) {
            formattedMessage.replace(pos, clientName.length(), BOLD + color + clientName + RESET + color);
        }
    }

    std::cout << color << formattedMessage << RESET << std::endl;
}



// Send raw message to client
void Server::sendRawMessageToClient(Client* client, const std::string& message)
{
	logToServer("Sending to " + client->getNickname() + RESET": " + message, "DEBUG");
	client->addToOutputBuffer(message);
	for (size_t i = 0; i < _pollDescriptors.size(); ++i)
	{
		if (_pollDescriptors[i].fd == client->getFd())
		{
			_pollDescriptors[i].events |= POLLOUT;
			break;
		}
	}
}

void Server::broadcastToChannel(Channel* channel, const std::string& message, Client* sender)
{
    const std::set<Client*>& clients = channel->getClients();
    for (std::set<Client*>::const_iterator it = clients.begin(); it != clients.end(); ++it) 
	{
        if (*it != sender) 
            sendRawMessageToClient(*it, message);
    }
}
