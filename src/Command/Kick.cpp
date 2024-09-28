#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

Kick::Kick() {}
Kick::~Kick() {}

void Kick::execute(Server& server, Client& client, const std::vector<std::string>& params)
{
	(void)client;
	(void)params;
	server.logToServer("KICK command received", "INFO");
}