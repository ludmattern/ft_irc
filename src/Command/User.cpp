#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

User::User() {}
User::~User() {}

void User::execute(Server& server, Client& client, const std::vector<std::string>& params)
{
	if (client.isRegistered())
	{
		server.sendError(&client, ERR_ALREADYREGISTRED, "", "You may not reregister");
		return;
	}

	if (params.size() < 4)
	{
		server.sendError(&client, ERR_NEEDMOREPARAMS, "USER", "Not enough parameters");
		return;
	}

	std::string username = params[0];
	std::string realname = params[3];

	client.setUsername(username);
	client.setRealname(realname);

	server.registerClientIfReady(&client);
}
