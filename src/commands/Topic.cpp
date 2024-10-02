#include "commands/Command.hpp"
#include "network/Server.hpp"
#include "network/Client.hpp"
#include "network/Channel.hpp"


Topic::Topic() {}

Topic::~Topic() {}

void Topic::execute(Client& client, const std::vector<std::string>& params) 
{
    (void)client;
    (void)params;
    std::cout << "Topic triggered\n";
}