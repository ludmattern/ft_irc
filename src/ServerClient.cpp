/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerClient.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 13:08:06 by fprevot           #+#    #+#             */
/*   Updated: 2024/10/01 15:17:41 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network/Server.hpp"
#include <arpa/inet.h>
#include <string>
#include <cstdio>
#include <iostream>
#include "replies.hpp"
void Server::clientConnect()
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
	_fds.push_back(client_pollfd);

	Client* new_client = new Client(client_fd, "localhost");

	_clients[client_fd] = new_client;

	std::cout << "New connection accepted, FD: " << client_fd << std::endl;
}


bool Server::extractCommand(int client_fd, std::string& command)
{
    std::string& clientBuffer = _clientInputBuffers[client_fd];
    size_t pos = clientBuffer.find("\r\n");
    if (pos != std::string::npos)
    {
        if (pos > MAX_MESSAGE_LENGTH)
        {
            return false;
        }
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
        // Accumuler les données reçues dans le buffer du client
        std::string& clientBuffer = _clientInputBuffers[client_fd];
        clientBuffer.append(buffer, bytes_read);
        std::string command;
        // Extraire les commandes complètes depuis le buffer
        while (extractCommand(client_fd, command))
        {
            log(command);
            processClientCommand(client, command);
        }
        return;
    }
    if (bytes_read == 0)
    {
        std::vector<std::string> params;
        _commandHandler->executeCommand("QUIT", *client, params);
        return;
    }
    if (errno != EWOULDBLOCK && errno != EAGAIN)
        throw std::runtime_error("Failed on recv");
}
