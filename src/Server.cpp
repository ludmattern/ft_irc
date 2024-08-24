/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 18:18:09 by lmattern          #+#    #+#             */
/*   Updated: 2024/08/21 17:42:19 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>
#include <stdexcept>
#include <cstdlib>

Server::Server(int port, const std::string &password) : _password(password)
{
	_Socket.bindSocket(port);
	_Socket.listenSocket();
	_Socket.setNonBlocking();

	pollfd server_pollfd;
	server_pollfd.fd = _Socket.getFd();
	server_pollfd.events = POLLIN;
	_SocketPollList.push_back(server_pollfd);

	std::cout << "Server started on port " << port << " with password protection" << std::endl;
}

Server::~Server()
{
	DisconnectClients();
}

void Server::start()
{
	while (true)
	{
		checkPoll();

		for (size_t i = 0; i < _SocketPollList.size(); i++)
			processPollEvent(_SocketPollList[i]);
	}
}

void Server::ProcessSignal(int signal)
{
	std::cout << "\nSignal " << signal << " received, shutting down server..." << std::endl;
	exit(0);
}

void Server::DisconnectClients()
{
	for (size_t i = 0; i < _SocketPollList.size(); ++i)
		close(_SocketPollList[i].fd);
	_SocketPollList.clear();
	_clients.clear();
	std::cout << "All clients disconnected" << std::endl;
}
