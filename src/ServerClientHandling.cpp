/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerClientHandling.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 18:09:38 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/01 11:12:10 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Command.hpp"
#include "cppLibft.hpp"
#include <arpa/inet.h>
#include <string>
#include <cstdio>
#include <iostream>

void Server::handleNewConnection()
{
	int client_fd;
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);

	client_fd = accept(_serverSocket, (struct sockaddr*)&client_addr, &client_len);
	if (client_fd < 0)
	{
		if (errno != EWOULDBLOCK && errno != EAGAIN)
			throw std::runtime_error("Failed on accept");
		return ;
	}

	setSocketNonBlocking(client_fd);

	struct pollfd client_pollfd;
	client_pollfd.fd = client_fd;
	client_pollfd.events = POLLIN;
	client_pollfd.revents = 0;
	_pollDescriptors.push_back(client_pollfd);

	Client* new_client = new Client(client_fd);
	char client_ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
	new_client->setIp(client_ip);
	_clients[client_fd] = new_client;

	std::cout << "New connection accepted, FD: " << client_fd << std::endl;
}

void Server::readFromClient(int client_fd)
{
	Client* client = _clients[client_fd];
	char buffer[512];
	int bytes_read;

	memset(buffer, 0, sizeof(buffer));
	bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
	if (bytes_read > 0)
	{
		client->appendToInputBuffer(buffer, bytes_read);
		std::string command;
		while (client->extractCommand(command))
		{
			logToServer("Command received from FD " + toString(client_fd) + ": " + command, "INFO");
			processClientCommand(client, command);
		}
		return ;
	}
	if (bytes_read == 0)
	{
		std::vector<std::string> params;
		_commandHandler->executeCommand("QUIT", *this, *client, params);
		return ;
	}
	if (errno != EWOULDBLOCK && errno != EAGAIN)
		throw std::runtime_error("Failed on recv");
}

void Server::writeToClient(int client_fd)
{
	Client* client = _clients[client_fd];
	const std::string& message = client->getOutputBuffer();

	int bytes_sent = send(client_fd, message.c_str(), message.size(), 0);
	if (bytes_sent < 0)
	{
		if (errno != EWOULDBLOCK && errno != EAGAIN)
			throw std::runtime_error("Failed on send");
		return ;
	}
	client->eraseFromOutputBuffer(bytes_sent);
	if (!(client->getOutputBuffer().empty()))
		return ;
}

void Server::closeClientConnection(int client_fd)
{
	if (_clients.find(client_fd) == _clients.end())
	{
		logToServer("Client FD " + toString(client_fd) + " not found", "ERROR");
		return;
	}

	Client* client = _clients[client_fd];

	std::string quitMessage = ":" + client->getPrefix() + " QUIT :Client disconnected" + CRLF;
	notifyChannelsAboutClientQuit(client, quitMessage);

	closeClientSocket(client_fd);
	
	removeClientFromPollDescriptors(client_fd);
	
	logToServer("Client " + toString(client_fd) + " disconnected.", "INFO");
}

void Server::notifyChannelsAboutClientQuit(Client* client, const std::string& message)
{
	const std::set<std::string> channelNames = client->getChannels();

	for (std::set<std::string>::const_iterator it = channelNames.begin(); it != channelNames.end(); ++it)
	{
		Channel* channel = getChannelByName(*it);
		if (!channel)
		{
			logToServer("Channel " + *it + " not found", "ERROR");
			continue;
		}

		broadcastToChannel(channel, message, client);

		channel->removeClient(client);
		client->leaveChannel(*it);

		if (channel->getClients().empty())
		{
			removeChannel(*it);
			delete channel;
		}
	}
}

void Server::closeClientSocket(int client_fd)
{
	Client* client = _clients[client_fd];
	close(client_fd);
	delete client;
	_clients.erase(client_fd);
}

void Server::removeClientFromPollDescriptors(int client_fd)
{
	for (size_t i = 0; i < _pollDescriptors.size(); ++i)
	{
		if (_pollDescriptors[i].fd == client_fd)
		{
			_pollDescriptors.erase(_pollDescriptors.begin() + i);
			break;
		}
	}
}
