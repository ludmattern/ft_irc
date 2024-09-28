#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

Notice::Notice() {}
Notice::~Notice() {}

void Notice::execute(Server& server, Client& client, const std::vector<std::string>& params)
{
	(void)client;
	(void)params;
	(void)server;

}
