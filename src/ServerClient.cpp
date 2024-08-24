/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerClient.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 17:31:12 by lmattern          #+#    #+#             */
/*   Updated: 2024/08/21 17:43:37 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>
#include <arpa/inet.h>

void Server::handleNewClient()
{
	sockaddr_in client_addr;
	socklen_t addrlen = sizeof(client_addr);

	try
	{
		int client_fd = _Socket.acceptClient(&client_addr, &addrlen);
		std::string ip_address = inet_ntoa(client_addr.sin_addr);

		Client new_client(client_fd, ip_address);
		_clients.push_back(new_client);

		pollfd client_pollfd;
		client_pollfd.fd = client_fd;
		client_pollfd.events = POLLIN;
		_SocketPollList.push_back(client_pollfd);

		std::cout << "New client connected: " << ip_address << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error accepting client: " << e.what() << std::endl;
	}
}

void Server::handleClientData(int client_fd)
{
	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));

	ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
	if (bytes_received <= 0)
	{
		handleClientDisconnection(client_fd);
		return ;
	}

	buffer[bytes_received] = '\0';
	std::string message(buffer);

	Client *client = findClientByFd(client_fd);
	if (!client)
	{
		std::cerr << "Error: Client not found." << std::endl;
		return ;
	}

	if (!client->isAuthenticated())
	{
		handleAuthentication(client, client_fd, message);
		return ;
	}

	std::cout << "Received from client " << client_fd << ": " << message << std::endl;

	// Traiter les commandes IRC standard ici (JOIN, NICK, etc.)
}

void Server::handleClientDisconnection(int client_fd)
{
	std::cout << "Client disconnected: " << client_fd << std::endl;
	removeClient(client_fd);
	close(client_fd);
}

Client *Server::findClientByFd(int client_fd)
{
	for (size_t i = 0; i < _clients.size(); ++i)
	{
		if (_clients[i].getFd() == client_fd)
			return (&_clients[i]);
	}
	return (NULL);
}

void Server::handleAuthentication(Client *client, int client_fd, const std::string &message)
{
	if (message.compare(0, 5, "PASS ") != 0)
	{
		std::cerr << "Client " << client_fd << " must authenticate first." << std::endl;
		return ;
	}

	std::string client_pass = message.substr(5);
	if (client_pass == _password)
	{
		client->authenticate();
		std::cout << "Client " << client_fd << " authenticated successfully." << std::endl;
	}
	else
	{
		std::cerr << "Client " << client_fd << " provided incorrect password." << std::endl;
		removeClient(client_fd);
		close(client_fd);
	}
}

void Server::removeClient(int fd)
{
	for (size_t i = 0; i < _SocketPollList.size(); ++i)
	{
		if (_SocketPollList[i].fd == fd)
		{
			_SocketPollList.erase(_SocketPollList.begin() + i);
			break ;
		}
	}

	for (size_t i = 0; i < _clients.size(); ++i)
	{
		if (_clients[i].getFd() == fd)
		{
			_clients.erase(_clients.begin() + i);
			break ;
		}
	}

	std::cout << "Client " << fd << " removed." << std::endl;
}
