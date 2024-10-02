#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include <sstream>
#include <vector>
#include <algorithm>

Quit::Quit() {}

Quit::~Quit() {}

void Quit::execute(Client& client, const std::vector<std::string>& params) 
{
    (void)client;
    (void)params;
    std::cout << "quit triggered\n";
}