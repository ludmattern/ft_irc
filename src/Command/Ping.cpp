#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

Ping::Ping() {}
Ping::~Ping() {}

void Ping::execute(Server& server, Client& client, const std::vector<std::string>& params)
{
	(void)client;
	(void)params;
	server.logToServer("PING command received", "INFO");
}