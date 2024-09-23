/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 10:53:23 by fprevot           #+#    #+#             */
/*   Updated: 2024/09/23 17:07:57 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <sstream>
#include <arpa/inet.h>
#include <csignal>

Server* global_server_instance = NULL;

// Constructor & Destructor
Server::Server(int port, const std::string& password)
	: _port(port), _password(password), _server_name("MyIRCServer"), _is_running(true)
{
	global_server_instance = this;
	
	init_server_socket();
	initializeCommandHandlers();
	signal(SIGINT, &Server::signal_handler);
	signal(SIGQUIT, &Server::signal_handler);
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

void Server::stop()
{
	this->_is_running = false;
    std::cout << "Server is shutting down..." << std::endl;
    
    // Fermer toutes les connexions
    for (size_t i = 0; i < _poll_fds.size(); ++i)
    {
        close(_poll_fds[i].fd);
    }

    // Libérer les clients
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
        delete it->second;
    
    _clients.clear();
    _poll_fds.clear();
    
    // Fermer le socket du serveur
    close(_server_fd);

    std::cout << "Server shutdowned" << std::endl;
}


// Initialize server socket
void Server::init_server_socket()
{
	_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_server_fd < 0)
	{
		throw std::runtime_error("Runtime error: Failed on socket creating");
	}

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

	set_non_blocking(_server_fd);

	struct pollfd server_pollfd;
	server_pollfd.fd = _server_fd;
	server_pollfd.events = POLLIN;
	server_pollfd.revents = 0;
	_poll_fds.push_back(server_pollfd);

	std::cout << "Server started on port " << _port << std::endl;
}

// Set socket to non-blocking
void Server::set_non_blocking(int fd)
{
	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
	{
		perror("Error setting non-blocking mode");
		close(fd);
		exit(EXIT_FAILURE);
	}
}

void Server::signal_handler(int signal)
{
    if (signal == SIGINT || signal == SIGQUIT)
    {
        std::cout << "Signal reçu (" << signal << "). Fermeture du serveur..." << std::endl;
		global_server_instance->setIsRunning(false);
    }
}

void Server::setIsRunning(bool is_running)
{
	this->_is_running = is_running;
}

// Run server
void Server::run()
{
	while (_is_running)
	{
		int poll_count = poll(&_poll_fds[0], _poll_fds.size(), -1);

		if (poll_count < 0)
		{
			throw std::runtime_error("Runtime error: Failed on poll");
		}

		for (size_t i = 0; i < _poll_fds.size(); ++i)
		{
			if (!_is_running) break;

			if (_poll_fds[i].fd == _server_fd && (_poll_fds[i].revents & POLLIN))
			{
				accept_new_connection();
			}
			else if (_poll_fds[i].revents & POLLIN)
			{
				client_read(_poll_fds[i].fd);
			}
			else if (_poll_fds[i].revents & POLLOUT)
			{
				client_write(_poll_fds[i].fd);
			}
			if (_poll_fds[i].fd != _server_fd && hasClientToBeDisconnected(_poll_fds[i].fd))
			{
				if (_poll_fds[i].revents & POLLOUT)
					client_write(_poll_fds[i].fd);
				disconnect_client(_poll_fds[i].fd);
			}
		}
	}
}

bool Server::hasClientToBeDisconnected(int client_fd)
{
	Client* client = _clients[client_fd];

	if (client->hasToDisconnect())
		return true;
	else
		return false;
}

// Accept new connections
void Server::accept_new_connection()
{
	int client_fd;
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);

	client_fd = accept(_server_fd, (struct sockaddr*)&client_addr, &client_len);
	if (client_fd < 0)
	{
		if (errno != EWOULDBLOCK && errno != EAGAIN)
		{
			throw std::runtime_error("Runtime error: Failed on accept");
		}
		return;
	}

	set_non_blocking(client_fd);

	struct pollfd client_pollfd;
	client_pollfd.fd = client_fd;
	client_pollfd.events = POLLIN;
	client_pollfd.revents = 0;
	_poll_fds.push_back(client_pollfd);

	Client* new_client = new Client(client_fd);
	// Set client IP
	char client_ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
	new_client->setIp(client_ip);
	_clients[client_fd] = new_client;

	std::cout << "New connection accepted, FD: " << client_fd << std::endl;
}

// Read from client
void Server::client_read(int client_fd)
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
			processCommand(client, command);
		}
	}
	else if (bytes_read == 0)
		client->setToDisconnect(true);
	else
	{
		if (errno != EWOULDBLOCK && errno != EAGAIN)
			throw std::runtime_error("Runtime error: Failed on recv");
	}
}

// Write to client
void Server::client_write(int client_fd)
{
	Client* client = _clients[client_fd];
	const std::string& message = client->getOutputBuffer();

	int bytes_sent = send(client_fd, message.c_str(), message.size(), 0);
	if (bytes_sent < 0)
		throw std::runtime_error("Runtime error: Failed on send");
	else
	{
		client->eraseFromOutputBuffer(bytes_sent);
		if (client->getOutputBuffer().empty())
		{
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
}

// Disconnect client
void Server::disconnect_client(int client_fd)
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

// Modify event to POLLOUT

// Initialize command handlers
void Server::initializeCommandHandlers()
{
	_commandHandlers["PASS"] = &Server::handlePassCommand;
	_commandHandlers["NICK"] = &Server::handleNickCommand;
	_commandHandlers["USER"] = &Server::handleUserCommand;
	// Add other commands here
}

// Process command
void Server::processCommand(Client* client, const std::string& commandLine)
{
	std::istringstream iss(commandLine);
	std::string command;
	iss >> command;

	std::string params;
	getline(iss, params);
	if (!params.empty() && params[0] == ' ')
		params.erase(0, 1); // Remove leading space

	std::map<std::string, CommandHandler>::iterator it = _commandHandlers.find(command);
	if (it != _commandHandlers.end())
	{
		CommandHandler handler = it->second;
		(this->*handler)(client, params);
	}
	else
	{
		sendError(client, ERR_UNKNOWNCOMMAND, command, "Unknown command");
	}
}

// Handle PASS command
void Server::handlePassCommand(Client* client, const std::string& params)
{
	if (client->isRegistered())
	{
		sendError(client, ERR_ALREADYREGISTRED, "", "You may not reregister");
		return;
	}

	if (params.empty())
	{
		sendError(client, ERR_NEEDMOREPARAMS, "PASS", "Not enough parameters");
		return;
	}
	client->setPassword(params);
}

// Handle NICK command
void Server::handleNickCommand(Client* client, const std::string& params)
{
	if (params.empty())
	{
		sendError(client, ERR_NONICKNAMEGIVEN, "", "No nickname given");
		return;
	}

	std::string nickname = params;

	if (isNicknameInUse(nickname))
	{
		sendError(client, ERR_NICKNAMEINUSE, nickname, "Nickname is already in use");
		return;
	}

	client->setNickname(nickname);

	// Check if client can be registered
	attemptClientRegistration(client);
}

// Check if nickname is in use
bool Server::isNicknameInUse(const std::string& nickname) const
{
	for (std::map<int, Client*>::const_iterator it = _clients.begin();
		it != _clients.end(); ++it)
	{
		if (it->second->getNickname() == nickname)
		{
			return true;
		}
	}
	return false;
}

// Handle USER command
void Server::handleUserCommand(Client* client, const std::string& params)
{
	if (client->isRegistered())
	{
		sendError(client, ERR_ALREADYREGISTRED, "", "You may not reregister");
		return;
	}

	std::istringstream iss(params);
	std::string username, hostname, servername, realname;

	iss >> username >> hostname >> servername;
	std::getline(iss, realname);

	if (username.empty() || hostname.empty() || servername.empty() || realname.empty())
	{
		sendError(client, ERR_NEEDMOREPARAMS, "USER", "Not enough parameters");
		return;
	}

	if (realname[0] == ':')
	{
		realname.erase(0, 1);
	}

	client->setUsername(username);
	client->setRealname(realname);

	attemptClientRegistration(client);
}

// Attempt to register client
void Server::attemptClientRegistration(Client* client)
{
	if (client->getNickname().empty() || client->getUsername().empty())
		return;

	if (!_password.empty())
	{
		if (client->getPassword() != _password)
		{
			sendError(client, ERR_PASSWDMISMATCH, "", "Password incorrect");
			client->setToDisconnect(true);
			return ;
		}
	}

	client->setRegistered(true);
	// Send welcome messages
	sendWelcomeMessages(client);
}

// Send welcome messages to client
void Server::sendWelcomeMessages(Client* client)
{
	std::string prefix = client->getPrefix();
	sendReply(client, RPL_WELCOME, "", "Welcome to the IRC server " + prefix);
	sendReply(client, RPL_YOURHOST, "", "Your host is " + _server_name + ", running version 1.0");
	sendReply(client, RPL_CREATED, "", "This server was created today");
	sendReply(client, RPL_MYINFO, _server_name + " 1.0", "Available user modes: ...");
}

// Send standard reply to client
void Server::sendReply(Client* client, const std::string& code,
					const std::string& params, const std::string& message)
{
	std::string nickname = client->getNickname();
	if (nickname.empty())
	{
		nickname = "*";
	}
	std::string reply = ":" + _server_name + " " + code + " " + nickname;
	if (!params.empty())
	{
		reply += " " + params;
	}
	if (!message.empty())
	{
		reply += " :" + message;
	}
	reply += "\r\n";
	sendToClient(client, reply);
}

// Send error to client
void Server::sendError(Client* client, const std::string& code,
					const std::string& command, const std::string& message)
{
	std::string nickname = client->getNickname();
	if (nickname.empty())
	{
		nickname = "*";
	}
	std::string error = ":" + _server_name + " " + code + " " + nickname;
	if (!command.empty())
	{
		error += " " + command;
	}
	error += " :" + message + "\r\n";
	sendToClient(client, error);
}

void Server::displaytoServer(const std::string& message)
{
	if (message.empty())
		return ;
	std::cout << message << std::endl;
}

// Send raw message to client
void Server::sendToClient(Client* client, const std::string& message)
{
	client->addToOutputBuffer(message);
	for (size_t i = 0; i < _poll_fds.size(); ++i)
	{
		if (_poll_fds[i].fd == client->getFd())
		{
			_poll_fds[i].events |= POLLOUT;
			break;
		}
	}
}
