/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 10:53:23 by fprevot           #+#    #+#             */
/*   Updated: 2024/09/30 09:15:04 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <sstream>
#include <arpa/inet.h>
#include <csignal>
#include <termios.h>
#include "cppLibft.hpp"
#include "Parser.hpp"
struct termios oldt, newt;

void disableControlCharacterEcho()
{
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;

	newt.c_lflag &= ~(ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

void restoreTerminalSettings()
{
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

Server* global_server_instance = NULL;

Server::Server(int argc, char **argv) : _serverName("MyIRCServer"), _isRunning(true)
{
	global_server_instance = this;
	
	_commandHandler = new Parser();
	disableControlCharacterEcho();
	parseArguments(argc, argv);
	initializeServerSocket();
	signal(SIGINT, &Server::handleSignal);
	signal(SIGQUIT, &Server::handleSignal);
}

Server::~Server()
{
	delete _commandHandler;
	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
		delete it->second;
	_channels.clear();
	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		close(it->first);
		delete it->second;
	}
	_clients.clear();
	for (size_t i = 0; i < _pollDescriptors.size(); ++i)
	{
		close(_pollDescriptors[i].fd);
	}
	_pollDescriptors.clear();
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

void Server::initializeServerSocket()
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
	_pollDescriptors.push_back(server_pollfd);

	logToServer("Server started on port " + toString(_port), "INFO");
}

void Server::handleSignal(int signal)
{
	if (signal == SIGINT || signal == SIGQUIT)
	{
		global_server_instance->logToServer("Signal received (" + toString(signal) + ")", "INFO");
		global_server_instance->logToServer("Closing server...", "WARNING");
		restoreTerminalSettings();
		global_server_instance->setRunningState(false);
	}
}

void Server::setRunningState(bool is_running)
{
	this->_isRunning = is_running;
}

void Server::run()
{
	while (_isRunning)
	{
		int poll_count = poll(&_pollDescriptors[0], _pollDescriptors.size(), -1);
		if (poll_count < 0)
		{
			if (_isRunning)
				throw std::runtime_error("Failed on poll");
			break;
		}

		for (size_t i = 0; i < _pollDescriptors.size(); ++i)
		{
			if (!_isRunning)
				break;
			handlePollEvent(_pollDescriptors[i]);
		}
	}
}

void Server::handlePollEvent(struct pollfd& pollDescriptor)
{
	if (isServerSocket(pollDescriptor))
	{
		if (pollDescriptor.revents & POLLIN)
			handleNewConnection();
	}
	else if (isClientSocket(pollDescriptor))
		checkClientEvents(pollDescriptor);
}

bool Server::isServerSocket(const struct pollfd& pollDescriptor) const
{
	return pollDescriptor.fd == _serverSocket;
}

bool Server::isClientSocket(const struct pollfd& pollDescriptor) const
{
	return pollDescriptor.fd != _serverSocket;
}

void Server::checkClientEvents(struct pollfd& pollDescriptor)
{
	if (pollDescriptor.revents & POLLIN)
		readFromClient(pollDescriptor.fd);

	if (pollDescriptor.revents & POLLOUT)
		writeToClient(pollDescriptor.fd);

	if (shouldClientDisconnect(pollDescriptor.fd))
		handleClientDisconnection(pollDescriptor);
}

void Server::handleClientDisconnection(struct pollfd& pollDescriptor)
{
	if (pollDescriptor.revents & POLLOUT)
		writeToClient(pollDescriptor.fd);
	closeClientConnection(pollDescriptor.fd);
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

bool Server::shouldClientDisconnect(int client_fd)
{
	Client* client = _clients[client_fd];

	return (client->shouldDisconnect());
}


Channel* Server::getChannelByName(const std::string& channelName)
{
    std::map<std::string, Channel*>::iterator it = _channels.find(channelName);
    if (it != _channels.end())
        return it->second;
    else
        return NULL;
}

Client* Server::getClientByNickname(const std::string& nickname)
{
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->second->getNickname() == nickname)
            return it->second;
    }
    return NULL;
}

void Server::removeChannel(const std::string& channelName) {
    _channels.erase(channelName);
}