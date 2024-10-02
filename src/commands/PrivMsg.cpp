#include "network/Server.hpp"
#include "network/Client.hpp"
#include "network/Channel.hpp"
#include "commands/Command.hpp"

PrivMsg::PrivMsg() {}

PrivMsg::~PrivMsg() {}

void PrivMsg::execute(Client& client, const std::vector<std::string>& params)
{
	(void)client;
	(void)params;
	std::cout << "Privmsg triggered\n";
}