/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 10:43:51 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/02 14:23:10 by lmattern         ###   ########.fr       */
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

	void addClient(Client& client);
	void welcomeClient(Client& client);
	void removeClient(Client& client);

	void setOperator(Client& client, bool isOperator);
	bool isOperator(Client& client) const;
	// void broadcast(const std::string& message);
	// void broadcast(const std::string& message, Client* sender);
	
	// void addClient(Client* client, bool isOperator);
	// void removeClient(Client* client);

private:
	std::string _name;
	std::string _topic;
	std::set<char> _modes;
	std::map<Client*, bool> _clients;
};

#endif
