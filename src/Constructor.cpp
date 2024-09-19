#include "../include/Server.hpp"

//Constructor & destructor



//SERVER
Server::Server(int port, const std::string &password) : _port(port), _password(password)
{
	init_server_socket();
}

Server::~Server()
{
	for (size_t i = 0; i < _poll_fds.size(); ++i)
	{
		close(_poll_fds[i].fd);
	}
}


//CLIENT
Client::Client(int fd) : _fd(fd) {}

Client::~Client() {}


//CHANNEL