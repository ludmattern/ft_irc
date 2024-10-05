/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 12:46:10 by fprevot           #+#    #+#             */
/*   Updated: 2024/10/05 17:57:49 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network/Server.hpp"
#include "network/Client.hpp"
#include "network/Channel.hpp"
#include "network/Parser.hpp"
#include "libs/cppLibft.hpp"
#include "replies.hpp"
#include <sstream>
#include <arpa/inet.h>
#include <csignal>
#include <termios.h>

struct termios oldt, newt;

void restoreTerminalSettings()
{
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

void Server::handleSignal(int signal)
{
	if (signal == SIGINT || signal == SIGQUIT)
	{
		std::string message = "Signal received (" + toString(signal) + ")";
		log(message);
		log("Closing server...");
		restoreTerminalSettings();
		Server& server = Server::getInstance();
		server._isRunning = false;
	}
}

void disableControlCharacterEcho()
{
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;

	newt.c_lflag &= ~(ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

Server::Server() : _isRunning(false), _name("ft_irc"), _maxClient(100) {}

void Server::init(int argc, char **argv) 
{
	if (!_isRunning)
	{
		parseArguments(argc, argv);
		initServer();
		disableControlCharacterEcho();
		parser = new Parser();
		_isRunning = true;
		signal(SIGINT, &Server::handleSignal);
		signal(SIGQUIT, &Server::handleSignal);
	}
	else
		throw std::runtime_error("Server is already initialized.");
}

Server::~Server()
{
	delete parser;
	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
		delete it->second;
	_channels.clear();
	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		close(it->first);
		delete it->second;
	}
	_clients.clear();
	for (size_t i = 0; i < _fds.size(); ++i)
		close(_fds[i].fd);
	_fds.clear();
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

bool Server::isServerSocket(const struct pollfd& pd) const {return pd.fd == _serverSocket;}

bool Server::isClientSocket(const struct pollfd& pd) const {return pd.fd != _serverSocket;}

std::string Server::getPassword() const {return _password;}

std::vector<Client*> Server::getClients() const
{
	std::vector<Client*> clients;
	for (std::map<int, Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
		clients.push_back(it->second);
	return clients;
}

Channel* Server::getChannel(const std::string& name)
{
	std::map<std::string, Channel*>::iterator it = _channels.find(name);
	if (it != _channels.end())
		return it->second;
	return NULL;
}

Channel* Server::addChannel(const std::string& name)
{
	Channel* channel = new Channel(name);
	_channels[name] = channel;
	return channel;
}

void Server::closeClientConnection(int client_fd)
{
	if (_clients.find(client_fd) == _clients.end())
	{
		log("Client FD " + toString(client_fd) + " not found");
		return;
	}
	Client* client = _clients[client_fd];
	std::string quitMessage = ":" + client->getPrefix() + " QUIT :Client disconnected" + CRLF;
	closeClientSocket(client_fd);
	removeClientFromPollDescriptors(client_fd);
	log("Client " + toString(client_fd) + " disconnected.");
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
	for (size_t i = 0; i < _fds.size(); ++i)
	{
		if (_fds[i].fd == client_fd)
		{
			_fds.erase(_fds.begin() + i);
			break;
		}
	}
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


void Server::removeChannel(const std::string& channelName)
{
	std::map<std::string, Channel*>::iterator it = _channels.find(channelName);
	if (it != _channels.end())
	{
		std::cout << "deleted\n";
		delete it->second;
		_channels.erase(it);
	}
}
