/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 18:21:43 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/01 11:50:39 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Channel.hpp"
#include "Client.hpp"
#include <string>
#include <map>
#include <vector>
#include <poll.h>

class Server {
public:

private:

	bool _isRunning;
	int _serverSocket;

	int _port;
	std::string _password;

	std::vector<pollfd> _fds;
	std::map<std::string, Channel*> _channels;
	std::map<int, Client*> _clients;

	void	clientConnect();
	void	clientDisconnect(int fd);
	void	clientRead(int fd);

	Channel* createChannel(const std::string& name);

	int initServer();
};

#endif
