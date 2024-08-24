/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 18:16:08 by lmattern          #+#    #+#             */
/*   Updated: 2024/08/21 17:36:05 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include <string>
#include <poll.h>
#include "Client.hpp"
#include "Socket.hpp"

#define ERROR -1
#define SUCCESS 0

class Server
{
private:
	Socket _Socket;
	std::vector<pollfd> _SocketPollList;
	std::vector<Client> _clients;
	std::string _password;

public:
	Server(int port, const std::string &password);
	~Server();

	void start();
	static void ProcessSignal(int signal);
	void DisconnectClients();
	static std::pair<int, std::string> parseArguments(int argc, char **argv);

private:
	void handleNewClient();
	void handleClientData(int client_fd);
	void removeClient(int fd);
	void checkPoll();
	void processPollEvent(pollfd& pfd);
	void handleClientDisconnection(int client_fd);
	Client *findClientByFd(int client_fd);
	void handleAuthentication(Client *client, int client_fd, const std::string &message);
};

#endif
