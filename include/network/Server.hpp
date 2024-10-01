/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 18:21:43 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/01 15:00:17 by fprevot          ###   ########.fr       */
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
	void run();
	Server(int argc, char **argv);
	~Server();

	Parser* _commandHandler;

private:


	bool _isRunning;
	int _serverSocket;

	int _port;
	std::string _password;

	std::vector<pollfd> _fds;
	std::map<std::string, Channel*> _channels;
	std::map<int, Client*> _clients;
    std::map<int, std::string> _clientInputBuffers;

	void	clientConnect();
	void	clientDisconnect(int fd);
	void	clientRead(int fd);

	Channel* createChannel(const std::string& name);

	void initServer();
	void setSocketNonBlocking(int fd);


	void parseArguments(int argc, char **argv);
	
	void handlePollEvent(struct pollfd& pd);
	bool isServerSocket(const struct pollfd& pd) const;
	bool isClientSocket(const struct pollfd& pd) const;

	//to parser
	bool extractCommand(int client_fd, std::string& command);
	void processClientCommand(Client *client, const std::string &commandLine);

};

#endif
