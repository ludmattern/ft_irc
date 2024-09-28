#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

Nick::Nick() {}
Nick::~Nick() {}

bool Server::isNicknameTaken(const std::string& nickname) const
{
	for (std::map<int, Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->second->getNickname() == nickname)
			return true;
	}
	return false;
}

void Server::registerClientIfReady(Client* client)
{
	if (client->getNickname().empty() || client->getUsername().empty())
		return;

	if (!_password.empty())
	{
		if (client->getPassword() != _password)
		{
			sendError(client, ERR_PASSWDMISMATCH, "", "Password incorrect");
			return ;
		}
	}

	client->setRegistered(true);
	sendWelcomeMessages(client);
}

void Nick::execute(Server& server, Client& client, const std::vector<std::string>& params)
{
	if (params.empty())
	{
		server.sendError(&client, ERR_NONICKNAMEGIVEN, "", "No nickname given");
		return;
	}

	std::string nickname = params[0];

	if (server.isNicknameTaken(nickname))
		server.sendError(&client, ERR_NICKNAMEINUSE, nickname, "Nickname is already in use");
	else
	{
		client.setNickname(nickname);
		server.registerClientIfReady(&client);
	}
}
