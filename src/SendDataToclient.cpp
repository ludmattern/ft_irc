#include "../include/Server.hpp"

/*
si y a un msg a send depuis le srv, le faire
*/

void Server::send_message_to_client(int client_fd, const std::string &message)
{
	Client *client = _clients[client_fd];
	client->add_to_output_buffer(message);
	mod_event_to_pollout(client_fd);
}

/*
Permet de passer un le bon clients en mode pollout pour envoyer le msg (cela sera detecter dans la boucle infinie de poll)
*/
void Server::mod_event_to_pollout(int client_fd)
{
	for (size_t i = 0; i < _poll_fds.size(); ++i)
	{
		if (_poll_fds[i].fd == client_fd) 
		{
			_poll_fds[i].events |= POLLOUT; 
		}
	}
}


/*
Envoyer le message au client, le delete et repasser le client en mode pollin, reception de donn2e
*/

void Server::client_write(int client_fd)
{
	Client *client = _clients[client_fd];
	const std::string &message = client->get_output_buffer();

	int bytes_sent = send(client_fd, message.c_str(), message.size(), 0);
	if (bytes_sent < 0)
	{
		perror("log : err on send");
		disconnect_client(client_fd);
	} 
	else
	{
		client->erase_from_output_buffer(bytes_sent);
		if (client->get_output_buffer().empty()) 
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
