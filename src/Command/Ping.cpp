#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

void Ping::execute(Server& server, Client& client, const std::vector<std::string>& params)
{
	if (params.empty())
	{
		server.sendError(&client, "409", "", "No origin specified");
		return;
	}
	server.sendRawMessageToClient(&client, "PONG :" + params[0] + "\r\n");
}