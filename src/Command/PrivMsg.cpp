#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

void PrivMsg::execute(Server& server, Client& client, const std::vector<std::string>& params)
{
	if (!client.isRegistered())
	{
		server.sendError(&client, "451", "PRIVMSG", "You are not registered");
		return;
	}

	if (params.size() < 2)
	{
		server.sendError(&client, "461", "PRIVMSG", "Not enough parameters");
		return;
	}

	std::string target = params[0];
	std::string message = params[1];

	if (!message.empty() && message[0] == ':')
		message.erase(0, 1);

	if (target[0] == '#' || target[0] == '&')
	{
		Channel* channel = server.getChannelByName(target);
		if (channel == NULL)
		{
			server.sendError(&client, "403", target, "No such channel");
			return;
		}

		if (!channel->hasClient(&client)) {
			server.sendError(&client, "404", target, "Cannot send to channel");
			return;
		}

		std::string fullMessage = ":" + client.getPrefix() + " PRIVMSG " + target + " :" + message + "\r\n";
		server.broadcastToChannel(channel, fullMessage, &client);
	}
	else
	{
		Client* recipient = server.getClientByNickname(target);
		if (recipient == NULL)
		{
			server.sendError(&client, "401", target, "No such nick/channel");
			return;
		}

		std::string fullMessage = ":" + client.getPrefix() + " PRIVMSG " + target + " :" + message + "\r\n";
		server.sendRawMessageToClient(recipient, fullMessage);
	}
}
