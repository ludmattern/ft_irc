#include "network/Server.hpp"
#include "network/Client.hpp"
#include "network/Channel.hpp"
#include "commands/Command.hpp"
#include "replies.hpp"

Part::Part() : Command() {}

Part::~Part() {}

void Part::execute(Client* client, const std::vector<std::string>& params) 
{
	if (client->getStatus() != REGISTERED) 
	{
		client->reply(ERR_NOTREGISTERED());
		return;
	}
	if (params.empty()) 
	{
		client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), std::string("PART")));
		return;
	}
	std::string channelsParam = params[0];
	std::stringstream ss(channelsParam);
	std::string channelName;

	std::string partMessage = (params.size() > 1) ? params[1] : "";

	while (std::getline(ss, channelName, ',')) 
	{
		Channel* channel = _server.getChannel(channelName);
		if (!channel) 
		{
			client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), channelName));
			continue;
		}
		if (!channel->hasClient(client)) 
		{
			client->reply(ERR_NOTONCHANNEL(client->getNickname(), channelName));
			continue;
		}
		std::string prefix = client->getPrefix();
		std::string message = ":" + prefix + " PART " + channelName;
		if (!partMessage.empty())
			message += " :" + partMessage;
		channel->broadcast(message);
		channel->removeClient(client);
		if (channel->getNumberOfClients() == 0)
			_server.removeChannel(channel->getName());
	}
}


