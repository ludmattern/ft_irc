/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSendings.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 10:53:23 by fprevot           #+#    #+#             */
/*   Updated: 2024/09/24 16:57:52 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <sstream>
#include <arpa/inet.h>
#include <csignal>

Server* global_server_instance = NULL;

Server::Server(int argc, char **argv)
	: _server_name("MyIRCServer"), _is_running(true)
{
	global_server_instance = this;

	parseArguments(argc, argv);
	initializeServerSocket();
	setupCommandHandlers();
	signal(SIGINT, &Server::handleSignal);
	signal(SIGQUIT, &Server::handleSignal);
}

Server::~Server()
{
	for (size_t i = 0; i < _poll_fds.size(); ++i)
	{
		close(_poll_fds[i].fd);
	}
	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		delete it->second;
	_clients.clear();
}

void Server::parseArguments(int argc, char **argv)
{
	if (argc != 3)
		throw std::invalid_argument("Usage: " + std::string(argv[0]) + " <listening port> <connection password>");
	char *endptr;
	long port = std::strtol(argv[1], &endptr, 10);
	if (*endptr != '\0' || endptr == argv[1])
		throw std::invalid_argument("Error: Port must be a valid integer.");
	if (port < MIN_PORT || port > MAX_PORT)
		throw std::invalid_argument("Error: Port must be a positive integer between 1 and 65535.");
	if (port < RESERVED_PORT)
		std::cerr << "Warning: Ports below 1024 are typically reserved for system services." << std::endl;
	std::string password = argv[2];

	_port = static_cast<int>(port);
	_password = password;
}

void Server::initializeServerSocket()
{
	_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_server_fd < 0)
		throw std::runtime_error("Runtime error: Failed on socket creating");

	int opt = 1;
	if (setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		close(_server_fd);
		throw std::runtime_error("Runtime error: Failed on setsocket");
	}

	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;          // IPv4
	server_addr.sin_addr.s_addr = INADDR_ANY;  // All local interfaces
	server_addr.sin_port = htons(_port);       // Convert port to network byte order

	if (bind(_server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		close(_server_fd);
		throw std::runtime_error("Runtime error: Failed on bind");
	}

	if (listen(_server_fd, SOMAXCONN) < 0)
	{
		close(_server_fd);
		throw std::runtime_error("Runtime error: Failed on listen");
	}

	setSocketNonBlocking(_server_fd);

	struct pollfd server_pollfd;
	server_pollfd.fd = _server_fd;
	server_pollfd.events = POLLIN;
	server_pollfd.revents = 0;
	_poll_fds.push_back(server_pollfd);

	std::cout << "Server started on port " << _port << std::endl;
}

void Server::handleSignal(int signal)
{
	if (signal == SIGINT || signal == SIGQUIT)
	{
		std::cout << " Signal received (" << signal << "). Closing server..." << std::endl;
		global_server_instance->setRunningState(false);
	}
}

void Server::setRunningState(bool is_running)
{
	this->_is_running = is_running;
}

void Server::run()
{
	while (_is_running)
	{
		int poll_count = poll(&_poll_fds[0], _poll_fds.size(), -1);
		if (poll_count < 0)
		{
			if (_is_running)
				throw std::runtime_error("Runtime error: Failed on poll");
			break ;
		}

		for (size_t i = 0; i < _poll_fds.size(); ++i)
		{
			if (!_is_running) break ;

			if (_poll_fds[i].fd == _server_fd && (_poll_fds[i].revents & POLLIN))
				handleNewConnection();
			else if (_poll_fds[i].revents & POLLIN)
				readFromClient(_poll_fds[i].fd);
			else if (_poll_fds[i].revents & POLLOUT)
				writeToClient(_poll_fds[i].fd);
			if (_poll_fds[i].fd != _server_fd && shouldClientDisconnect(_poll_fds[i].fd))
			{
				if (_poll_fds[i].revents & POLLOUT)
					writeToClient(_poll_fds[i].fd);
				closeClientConnection(_poll_fds[i].fd);
			}
		}
	}
}

void Server::setSocketNonBlocking(int fd)
{
	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
	{
		perror("Error setting non-blocking mode");
		close(fd);
		exit(EXIT_FAILURE);
	}
}

bool Server::shouldClientDisconnect(int client_fd)
{
	Client* client = _clients[client_fd];

	if (client->shouldDisconnect())
		return true;
	else
		return false;
}
