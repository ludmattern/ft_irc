#include "commands/Command.hpp"
#include "network/Server.hpp"
#include "network/Client.hpp"
#include "network/Channel.hpp"
#include <sstream>
#include <vector>
#include <algorithm>

Join::Join() {}

Join::~Join() {}

void Join::execute(Client& client, const std::vector<std::string>& params) 
{
    (void)client;
    (void)params;
    std::cout << "Join triggered\n";
}