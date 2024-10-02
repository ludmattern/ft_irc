#include "network/Server.hpp"
#include "network/Client.hpp"
#include "network/Channel.hpp"
#include "commands/Command.hpp"

Invite::Invite() {}

Invite::~Invite() {}

void Invite::execute(Client& client, const std::vector<std::string>& params)
{

	(void)client;
	(void)params;
	std::cout << "WORKING\n";
}