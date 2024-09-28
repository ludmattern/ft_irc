/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerClientHandling.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 18:09:38 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/28 15:22:11 by lmattern         ###   ########.fr       */
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
			throw std::runtime_error("Runtime error: Failed on accept");
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
	}
	else if (bytes_read == 0)
	{
        std::vector<std::string> params;
		_commandHandler->executeCommand("QUIT", *this, *client, params);
	}
	else
	{
		if (errno != EWOULDBLOCK && errno != EAGAIN)
			throw std::runtime_error("Runtime error: Failed on recv");
	}
}

void Server::writeToClient(int client_fd)
{
	Client* client = _clients[client_fd];
	const std::string& message = client->getOutputBuffer();

	int bytes_sent = send(client_fd, message.c_str(), message.size(), 0);
	if (bytes_sent < 0)
	{
		if (errno != EWOULDBLOCK && errno != EAGAIN)
			throw std::runtime_error("Runtime error: Failed on send");
	}
	else
	{
		client->eraseFromOutputBuffer(bytes_sent);
		if (client->getOutputBuffer().empty())
		{
			for (size_t i = 0; i < _pollDescriptors.size(); ++i)
			{
				if (_pollDescriptors[i].fd == client_fd)
				{
					_pollDescriptors[i].events = POLLIN;
					break;
				}
			}
		}
	}
}


void Server::closeClientConnection(int client_fd) {
    Client* client = _clients[client_fd];

    // Notify all channels about the client quitting
    const std::set<std::string>& channels = client->getChannels();
    std::string prefix = client->getPrefix();
    std::string message = ":" + prefix + " QUIT :Client disconnected\r\n";

    for (std::set<std::string>::const_iterator it = channels.begin(); it != channels.end(); ++it) {
        Channel* channel = getChannelByName(*it);
        if (channel) {
            broadcastToChannel(channel, message, client);
            channel->removeClient(client);
            if (channel->getClients().empty()) {
                removeChannel(*it);
                delete channel;
            }
        }
    }

    // Close the socket and remove the client
    close(client_fd);
    delete client;
    _clients.erase(client_fd);

    // Remove from poll descriptors
    for (size_t i = 0; i < _pollDescriptors.size(); ++i) {
        if (_pollDescriptors[i].fd == client_fd) {
            _pollDescriptors.erase(_pollDescriptors.begin() + i);
            break;
        }
    }

    logToServer("Client " + toString(client_fd) + " disconnected.", "INFO");
}