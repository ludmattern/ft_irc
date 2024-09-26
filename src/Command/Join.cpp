#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

void Server::sendChannelInfoToClient(Channel* channel, Client* client)
{
	// Send the topic if it exists
	if (!channel->getTopic().empty())
		sendReply(client, RPL_TOPIC, channel->getName(), channel->getTopic());
	else
		sendReply(client, RPL_NOTOPIC, channel->getName(), "No topic is set");

	// Build the names list
	std::string namesList;
	for (std::set<Client*>::const_iterator it = channel->getClients().begin(); it != channel->getClients().end(); ++it)
		namesList += (*it)->getNickname() + " ";

	// Remove trailing space
	if (!namesList.empty())
		namesList.erase(namesList.size() - 1);

	// :<server> 353 <nick> <symbol> <channel> :<names list>
	std::string symbol = "="; // "=" denotes a public channel
	std::string params = symbol + " " + channel->getName();
	sendReply(client, RPL_NAMREPLY, params, namesList);

	sendReply(client, RPL_ENDOFNAMES, channel->getName(), "End of /NAMES list");

	logToServer("Client " + client->getNickname() + " joined channel " + channel->getName(), "INFO");
}

Channel* Server::getOrCreateChannel(const std::string& channelName)
{
	std::map<std::string, Channel*>::iterator it = _channels.find(channelName);
	if (it == _channels.end()) {
		Channel* newChannel = new Channel(channelName);
		_channels[channelName] = newChannel;
		return newChannel;
	}
	return it->second;
}

void Server::addClientToChannel(Channel* channel, Client* client)
{
	channel->addClient(client);
	client->joinChannel(channel->getName());

	std::string joinMessage = ":" + client->getPrefix() + " JOIN :" + channel->getName() + "\r\n";
	sendRawMessageToClient(client, joinMessage);
	channel->broadcastMessage(joinMessage, client);
}

void Join::execute(Server& server, Client& client, const std::vector<std::string>& params)
{
	if (!client.isRegistered())
	{
		server.sendError(&client, "451", "JOIN", "You are not registered");
		return;
	}

	if (params.empty())
	{
		server.sendError(&client, ERR_NEEDMOREPARAMS, "JOIN", "Not enough parameters");
		return;
	}

	std::string channelName = params[0];

	Channel* channel = server.getOrCreateChannel(channelName);

	server.addClientToChannel(channel, &client);
	server.sendChannelInfoToClient(channel, &client);
}
