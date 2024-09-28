#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

Nick::Nick() {}
Nick::~Nick() {}

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
