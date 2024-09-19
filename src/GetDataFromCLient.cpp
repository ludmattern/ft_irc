#include "../include/Server.hpp"

/*
on accept les co et on met le client en mode non bloquqnt et on l add a la liste des clients

PEtit msg d accueil envoye pour tester le POLLOUT write
*/

void Server::accept_new_connection()
{
	int client_fd;
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);

	client_fd = accept(_server_fd, (struct sockaddr *)&client_addr, &client_len);
	if (client_fd < 0)
	{
		if (errno != EWOULDBLOCK && errno != EAGAIN)
		{
			perror("log : err on accept");
		}
		return;
	}
	set_non_blocking(client_fd);

	struct pollfd client_pollfd;
	client_pollfd.fd = client_fd;
	client_pollfd.events = POLLIN;
	client_pollfd.revents = 0;
	_poll_fds.push_back(client_pollfd);

	Client *new_client = new Client(client_fd);
	_clients[client_fd] = new_client;

	std::cout << "New conection accpeted on, FD : " << client_fd << std::endl;

	std::string welcome_message = "Welcome to the IRC server!\r\n";
	new_client->add_to_output_buffer(welcome_message);
	mod_event_to_pollout(client_fd);
}

/*
Gerer le POLLIN la reception depuis le clients et le mettre dans le buffer (buffer de 512 car max possible en irc voir pour opti plus tard si possible)

une fois les donne recue execute les cmd si y en a
*/

void Server::client_read(int client_fd)
{
	Client *client = _clients[client_fd];
	char buffer[512];
	int bytes_read;

	memset(buffer, 0, sizeof(buffer));
	bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
	if (bytes_read == 0)
	{
		std::cout << "Client FD " << client_fd << " closed connection." << std::endl;
		disconnect_client(client_fd);
	} 
	else
	{
		client->append_to_buffer(buffer, bytes_read);
		std::string command;
		while (client->extract_command(command))
		{
			std::cout << "Command received from " << client_fd << " : " << command << std::endl;
			//ici execute les cmd recue si y en a
			std::string response = "Command received: " + command + "\r\n";
			client->add_to_output_buffer(response);
			mod_event_to_pollout(client_fd);
		}
	}
}