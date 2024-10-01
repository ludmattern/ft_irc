/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 12:46:10 by fprevot           #+#    #+#             */
/*   Updated: 2024/10/01 16:48:09 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network/Server.hpp"
#include "network/Client.hpp"
#include <sstream>
#include <arpa/inet.h>
#include <csignal>
#include <termios.h>
#include "network/Parser.hpp"

Server::Server(int argc, char **argv) : _isRunning(true)
{	
	parseArguments(argc, argv);
	initServer();
	parser = new Parser();
}

Server::~Server()
{
}

void Server::parseArguments(int argc, char **argv)
{
	if (argc != 3)
		throw std::invalid_argument("Usage: " + std::string(argv[0]) + " <listening port> <connection password>");
	char *endptr;
	long port = std::strtol(argv[1], &endptr, 10);
	if (*endptr || endptr == argv[1])
		throw std::invalid_argument("Port must be a valid integer.");
	if (port < MIN_PORT || port > MAX_PORT)
		throw std::invalid_argument("Port must be a positive integer between 1 and 65535.");
	if (port < RESERVED_PORT)
		std::cerr << "Warning: Ports below 1024 are typically reserved for system services." << std::endl;
	std::string password = argv[2];
	if (password.empty())
		throw std::invalid_argument("Password must not be empty.");
	_password = password;
	_port = static_cast<int>(port);
}


void Server::initServer()
{
	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocket < 0)
		throw std::runtime_error("Failed on socket creating");
	int opt = 1;
	if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		close(_serverSocket);
		throw std::runtime_error("Failed on setsocket");
	}

	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;          // IPv4
	server_addr.sin_addr.s_addr = INADDR_ANY;  // All local interfaces
	server_addr.sin_port = htons(_port);       // Convert port to network byte order

	if (bind(_serverSocket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		close(_serverSocket);
		throw std::runtime_error("Failed on bind");
	}

	if (listen(_serverSocket, SOMAXCONN) < 0)
	{
		close(_serverSocket);
		throw std::runtime_error("Failed on listen");
	}

	setSocketNonBlocking(_serverSocket);

	struct pollfd server_pollfd;
	server_pollfd.fd = _serverSocket;
	server_pollfd.events = POLLIN;
	server_pollfd.revents = 0;
	_fds.push_back(server_pollfd);

}

void Server::setSocketNonBlocking(int fd)
{
	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
	{
		perror("Error setting non-blocking mode");
		close(fd);
		throw std::runtime_error("Failed on fcntl");
	}
}


void Server::run()
{
	while (_isRunning)
	{
		int poll_count = poll(&_fds[0], _fds.size(), -1);
		if (poll_count < 0)
		{
			if (_isRunning)
				throw std::runtime_error("Failed on poll");
			break;
		}

		for (size_t i = 0; i < _fds.size(); ++i)
		{
			if (!_isRunning)
				break;
			handlePollEvent(_fds[i]);
		}
	}
}



void Server::handlePollEvent(struct pollfd& pd)
{
	if (isServerSocket(pd))
	{
		if (pd.revents & POLLIN)
			clientConnect();
	}
	else if (isClientSocket(pd))
	{
		if (pd.revents & POLLIN)
			clientRead(pd.fd);
	}
}

bool Server::isServerSocket(const struct pollfd& pd) const
{
	return pd.fd == _serverSocket;
}

bool Server::isClientSocket(const struct pollfd& pd) const
{
	return pd.fd != _serverSocket;
}

std::vector<Client*> Server::getClients() const
{
	std::vector<Client*> clients;
	for (std::map<int, Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
		clients.push_back(it->second);
	return clients;
}