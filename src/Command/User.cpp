#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

void User::execute(Server& server, Client& client, const std::vector<std::string>& params)
{
	if (client.isRegistered())
	{
		server.sendError(&client, "462", "", "You may not reregister");
		return;
	}

	if (params.size() < 4)
	{
		server.sendError(&client, "461", "USER", "Not enough parameters");
		return;
	}

	std::string username = params[0];
	std::string realname = params[3];

	client.setUsername(username);
	client.setRealname(realname);

	server.registerClientIfReady(&client);
}
