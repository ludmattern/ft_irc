/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 18:21:43 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/05 16:44:28 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#define MIN_PORT 1
#define MAX_PORT 65535
#define RESERVED_PORT 1024

#define MAX_BUFFER_SIZE 512
#define MAX_MESSAGE_LENGTH 510

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <map>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "network/Client.hpp"
#include "network/Channel.hpp"
#include "network/Parser.hpp"

class Server {
public:

	static Server& getInstance()
	{
		static Server instance;
		return instance;
	}

	void init(int argc, char **argv);
	
	void run();

	Channel* addChannel(const std::string& name);
	Channel* getChannel(const std::string& name);
	std::map<std::string, Channel*> getChannels() const;

	std::vector<Client*> getClients() const;
	std::string getPassword() const;
	std::string getName() const { return _name; }

	void closeClientConnection(int client_fd);
	Client* getClientByNickname(const std::string& nickname);

	Parser* parser;

private:

	Server();
	~Server();

	bool _isRunning;
	int _serverSocket;
	std::string _name;

	int _port;
	std::string _password;
	
	size_t _maxClient;
	std::vector<pollfd> _fds;
	std::map<std::string, Channel*> _channels;
	std::map<int, Client*> _clients;

	void	clientConnect();
	void	clientDisconnect(int fd);
	void	clientRead(int fd);


	void initServer();
	void setSocketNonBlocking(int fd);


	void parseArguments(int argc, char **argv);
	
	void handlePollEvent(struct pollfd& pd);
	bool isServerSocket(const struct pollfd& pd) const;
	bool isClientSocket(const struct pollfd& pd) const;

	/* Signals */
	static void handleSignal(int signal);
	void closeClientSocket(int client_fd);
	void removeClientFromPollDescriptors(int client_fd);
};

#endif
