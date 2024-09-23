/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerClientHandling.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 18:09:38 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/23 18:57:49 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <arpa/inet.h>
#include <string>
#include <cstdio>
#include <iostream>

void Server::handleNewConnection()
{
	int client_fd;
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);

	client_fd = accept(_server_fd, (struct sockaddr*)&client_addr, &client_len);
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
	_poll_fds.push_back(client_pollfd);

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
			std::cout << "Command received from FD " << client_fd << ": " << command << std::endl;
			// Process received commands
			processClientCommand(client, command);
		}
	}
	else if (bytes_read == 0)
		client->markForDisconnection(true);
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
		throw std::runtime_error("Runtime error: Failed on send");
	else
	{
		client->eraseFromOutputBuffer(bytes_sent);
		if (!(client->getOutputBuffer().empty()))
			return ;
		for (size_t i = 0; i < _poll_fds.size(); ++i)
		{
			if (_poll_fds[i].fd == client_fd)
			{
				_poll_fds[i].events = POLLIN;
				break;
			}
		}
	}
}

void Server::closeClientConnection(int client_fd)
{
	close(client_fd);
	delete _clients[client_fd];
	_clients.erase(client_fd);
	for (size_t i = 0; i < _poll_fds.size(); ++i)
	{
		if (_poll_fds[i].fd == client_fd)
		{
			_poll_fds.erase(_poll_fds.begin() + i);
			break;
		}
	}
	std::cout << "Client " << client_fd << " disconnected." << std::endl;
}