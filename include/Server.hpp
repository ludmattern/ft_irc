/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 13:47:38 by fprevot           #+#    #+#             */
/*   Updated: 2024/09/19 14:39:40 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

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

#include "Client.hpp"

class Server {
public:
	Server(int port, const std::string &password);
	~Server();

	void run();

private:
	int _server_fd;
	int _port;
	std::string _password;
	std::vector<struct pollfd> _poll_fds;
	std::map<int, Client*> _clients; 
	
	void mod_event_to_pollout(int client_fd);
	void send_message_to_client(int client_fd, const std::string &message);
	void init_server_socket();
	void set_non_blocking(int fd);
	void accept_new_connection();
	void client_read(int client_fd);
	void client_write(int client_fd);
	void disconnect_client(int client_fd);
};

#endif 
