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
		client->reply(ERR_NOTREGISTERED(client->getNickname()));
		return;
	}
	if (params.empty()) 
	{
		client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "PART"));
		return;
	}
	std::string channelsParam = params[0];
	std::stringstream ss(channelsParam);
	std::string channelName;

	std::string partMessage = (params.size() > 1) ? params[1] : "";

	while (std::getline(ss, channelName, ',')) 
	{
		Channel* channel = _server.getChannelByName(channelName);
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
		{
			message += " :" + partMessage;
		}
		message += CRLF;
		channel->broadcast(message);

		channel->removeClient(client);
		client->partChannel(channel);
	}
}