#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
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