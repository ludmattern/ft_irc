#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

void PrivMsg::execute(Server& server, Client& client, const std::vector<std::string>& params)
{
	if (!client.isRegistered())
	{
		server.sendError(&client, ERR_NOTREGISTERED, "PRIVMSG", "You are not registered");
		return;
	}

	if (params.size() < 2)
	{
		server.sendError(&client, ERR_NEEDMOREPARAMS, "PRIVMSG", "Not enough parameters");
		return;
	}

	std::string target = params[0];
	std::string message = params[1];
	if (message[0] == ':')
		message.erase(0, 1);

	if (target[0] == GLOBAL_CHANNEL || target[0] == LOCAL_CHANNEL)
	{
		Channel* channel = server.getChannelByName(target);
		if (channel == NULL)
		{
			server.sendError(&client, ERR_NOSUCHCHANNEL, target, "No such channel");
			return;
		}
		if (!channel->hasClient(&client)) 
		{
			server.sendError(&client, ERR_CANNOTSENDTOCHAN, target, "Cannot send to channel");
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
			server.sendError(&client, ERR_NOSUCHNICK, target, "No such nick/channel");
			return;
		}
		std::string fullMessage = ":" + client.getPrefix() + " PRIVMSG " + target + " :" + message + "\r\n";
		server.sendRawMessageToClient(recipient, fullMessage);
	}
}
