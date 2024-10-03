/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 10:43:51 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/03 17:28:34 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <set>
#include <map>
#include "network/Client.hpp"

class Client;

class Channel {
public:

	Channel(const std::string& name);

	void addClient(Client* client);
	void addClient(Client* client, bool isOperator);
	void welcomeClient(Client* client);
	void removeClient(Client* client);

	void setOperator(Client* client, bool isOperator);
	bool isOperator(Client* client) const;
	void broadcast(const std::string& message);
	void broadcast(const std::string& message, Client* sender);

	bool hasClient(Client* client) const {return _clients.find(client) != _clients.end();}
	bool hasMode(char mode) const { return _modes.find(mode) != _modes.end();}
	std::string getName() const {return _name;}
	std::string getTopic() const {return _topic;}
	int getNumberOfClients() const {return _clients.size();}
	std::string getUserList() const;
	int getLimit() const {return _limit;}
    std::string getPassword() const;

	void setLimit(int limit) { _limit = limit;}
	void setTopic(const std::string& topic) {_topic = topic;}
	void setPassword(const std::string& password);
    void setMode(char mode) {_modes.insert(mode);}
	void removeMode(char mode) {_modes.erase(mode);}

	void addInvite(Client& client);
	
	void sendNamesList(Client& client);

private:
	int			_limit;
	std::string _name;
	std::string _topic;
	std::set<char> _modes;
	std::map<Client*, bool> _clients;
	std::set<Client*> _invitedClients;
	std::string _password;
};

#endif
