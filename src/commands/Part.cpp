#include "network/Server.hpp"
#include "network/Client.hpp"
#include "network/Channel.hpp"
#include "commands/Command.hpp"
#include "replies.hpp"

Part::Part() {}

Part::~Part() {}

void Part::execute(Client& client, const std::vector<std::string>& params)
{
	(void)client;
	(void)params;
	std::cout << "Part triggered\n";
}