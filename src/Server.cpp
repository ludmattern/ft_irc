

#include "../include/Server.hpp"

/*
Creation du socket et lancement du server sur les port etc..
*/

void Server::init_server_socket() 
{
	_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_server_fd < 0)
	{
		perror("log : err on socket creation");
		exit(EXIT_FAILURE);
	}
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_port = htons(_port);
	int opt = 1;
	if (setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		perror("log : err on setsockopt");
		close(_server_fd);
		exit(EXIT_FAILURE);
	}
	if (bind(_server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("log : err on bind");
		close(_server_fd);
		exit(EXIT_FAILURE);
	}
	if (listen(_server_fd, SOMAXCONN) < 0)
	{
		perror("log : err on listen");
		close(_server_fd);
		exit(EXIT_FAILURE);
	}
	set_non_blocking(_server_fd);
	struct pollfd server_pollfd;
	server_pollfd.fd = _server_fd;
	server_pollfd.events = POLLIN;
	server_pollfd.revents = 0;
	_poll_fds.push_back(server_pollfd);
	std::cout << "Server started on port " << _port << std::endl;
}

/*
JE le passe en mode non bloauqnd
*/

void Server::set_non_blocking(int fd) 
{
	if (fcntl(fd, F_SETFL, O_NONBLOCK)  < 0)
	{
		perror("log : err on FCNTL");
		close(fd);
		exit(EXIT_FAILURE);
	}
}
/*
La boucle infinie avec poll() et les connection la lecture des clients et leurs ecritureS  pour chaque fd
*/

void Server::run() 
{
	while (true) 
	{
		int poll_count = poll(&_poll_fds[0], _poll_fds.size(), -1);

		if (poll_count < 0)
		{
			perror("log : err on poll");
			break;
		}
		for (size_t i = 0; i < _poll_fds.size(); ++i)
		{
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
		}
	}
}

/*
tej le iencli
*/
void Server::disconnect_client(int client_fd) 
{
	close(client_fd);
	delete _clients[client_fd];
	_clients.erase(client_fd);
	for (size_t i = 0; i < _poll_fds.size(); ++i)
	{
		if (_poll_fds[i].fd == client_fd) {
			_poll_fds.erase(_poll_fds.begin() + i);
			break;
		}
	}
	std::cout << "Client " << client_fd << " disconected." << std::endl;
}

