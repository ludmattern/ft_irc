/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerClient.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 13:08:06 by fprevot           #+#    #+#             */
/*   Updated: 2024/10/06 16:19:28 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network/Server.hpp"
#include "replies.hpp"
#include <arpa/inet.h>
#include <string>
#include <cstdio>
#include <iostream>
#include "replies.hpp"

void Server::clientConnect()
{
	if(_fds.size() > _maxClient)
		return;
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
	_fds.push_back(client_pollfd);

	Client* new_client = new Client(client_fd, "localhost");

	_clients[client_fd] = new_client;

	std::cout << "New connection accepted, FD: " << client_fd << std::endl;
}


bool Parser::extractCommand(int client_fd, std::string& command)
{
	std::string& clientBuffer = _clientInputBuffers[client_fd];
	size_t pos = clientBuffer.find(CRLF);
	if (pos != std::string::npos)
	{
		if (pos > MAX_MESSAGE_LENGTH)
			return false;
		command = clientBuffer.substr(0, pos);
		clientBuffer.erase(0, pos + 2);
		return true;
	}
	return false;
}



void Server::clientRead(int client_fd)
{
	Client* client = _clients[client_fd];
	char buffer[512];
	int bytes_read;

	memset(buffer, 0, sizeof(buffer));
	bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
	if (bytes_read > 0)
	{
		std::string& clientBuffer = parser->_clientInputBuffers[client_fd];
		clientBuffer.append(buffer, bytes_read);
		std::string command;
		std::string cmdName;
		while (parser->extractCommand(client_fd, command))
		{
			log(command);
			parser->processClientCommand(client, command);
		}
		return;
	}
	else if (bytes_read == 0)
	{
		std::vector<std::string> params;
		parser->executeCommand("QUIT", client, params);
		return;
	}
	else if (errno != EWOULDBLOCK && errno != EAGAIN)
		throw std::runtime_error("Failed on recv");
}
