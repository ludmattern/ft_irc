/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 18:15:29 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/24 15:35:50 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::sendWelcomeMessages(Client* client)
{
	std::string prefix = client->getPrefix();
	sendReply(client, RPL_WELCOME, "", "Welcome to the IRC server " + prefix);
	sendReply(client, RPL_YOURHOST, "", "Your host is " + _server_name + ", running version 1.0");
	sendReply(client, RPL_CREATED, "", "This server was created today");
	sendReply(client, RPL_MYINFO, _server_name + " 1.0", "Available user modes: ...");
}

// Send standard reply to client
void Server::sendReply(Client* client, const std::string& code,
					const std::string& params, const std::string& message)
{
	std::string nickname = client->getNickname();
	if (nickname.empty())
		nickname = "*";
	std::string reply = ":" + _server_name + " " + code + " " + nickname;
	if (!params.empty())
		reply += " " + params;
	if (!message.empty())
		reply += " :" + message;
	reply += "\r\n";
	sendRawMessageToClient(client, reply);
}

// Send error to client
void Server::sendError(Client* client, const std::string& code,
					const std::string& command, const std::string& message)
{
	std::string nickname = client->getNickname();
	if (nickname.empty())
		nickname = "*";
	std::string error = ":" + _server_name + " " + code + " " + nickname;
	if (!command.empty())
		error += " " + command;
	error += " :" + message + "\r\n";
	sendRawMessageToClient(client, error);
}

void Server::logToServer(const std::string& message)
{
	if (message.empty())
		return ;
	std::cout << message << std::endl;
}

// Send raw message to client
void Server::sendRawMessageToClient(Client* client, const std::string& message)
{
	logToServer("Sending to " + client->getNickname() + ": " + message);
	client->addToOutputBuffer(message);
	for (size_t i = 0; i < _poll_fds.size(); ++i)
	{
		if (_poll_fds[i].fd == client->getFd())
		{
			_poll_fds[i].events |= POLLOUT;
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
		{
            sendRawMessageToClient(*it, message);
        }
    }
}
