#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include <sstream>
#include <vector>
#include <algorithm>

Quit::Quit() {}

Quit::~Quit() {}

void Quit::execute(Server& server, Client& client, const std::vector<std::string>& params) {
    std::string quitMessage = "Client exited";
    if (!params.empty()) {
        quitMessage = params[0];
    }

    const std::set<std::string>& channels = client.getChannels();
    std::string prefix = client.getPrefix();
    std::string message = ":" + prefix + " QUIT";
    if (!quitMessage.empty()) {
        message += " :" + quitMessage;
    }
    message += "\r\n";

    for (std::set<std::string>::const_iterator it = channels.begin(); it != channels.end(); ++it) {
        Channel* channel = server.getChannelByName(*it);
        if (channel) {
            server.broadcastToChannel(channel, message, &client);
            channel->removeClient(&client);
            if (channel->getClients().empty()) {
                server.removeChannel(*it);
                delete channel;
            }
        }
    }

    client.markForDisconnection(true);
}
