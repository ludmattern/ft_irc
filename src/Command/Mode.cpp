#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

void Mode::execute(Server& server, Client& client, const std::vector<std::string>& params)
{
	(void)client;
	(void)params;
	server.logToServer("QUIT command received", "Mode");
}
