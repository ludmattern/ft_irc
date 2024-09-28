#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

Pong::Pong() {}
Pong::~Pong() {}

void Pong::execute(Server& server, Client& client, const std::vector<std::string>& params)
{
	(void)client;
	(void)params;
	server.logToServer("PONG command received", "INFO");
}
