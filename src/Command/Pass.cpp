#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

void Pass::execute(Server& server, Client& client, const std::vector<std::string>& params) 
{
    if (client.isRegistered())
        server.sendError(&client, "462", "", "You may not reregister");
    else if (params.empty())
        server.sendError(&client, "461", "PASS", "Not enough parameters");
    else
        client.setPassword(params[0]);
}

