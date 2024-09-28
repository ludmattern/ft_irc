/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 10:43:51 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/28 16:33:23 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <set>
#include "Client.hpp"

class Client;

class Channel {
public:
	// Constructeur
	Channel(Client& client, const std::string& name);
	~Channel();

	// Getters
	const std::string& getName() const;
	const std::string& getTopic() const;

	// Setters
	void setTopic(const std::string& topic);

	// Gestion des clients
	void addClient(Client* client);
	void removeClient(Client* client);
	bool hasClient(Client* client) const;
	const std::set<Client*>& getClients() const;

	// Gestion des operateur
	void addOperator(Client* client);
	void removeOperator(Client* client);
	bool hasOperator(Client* client) const;
	const std::set<Client*>& getOperators() const;

	// Gestion des modes
	void addMode(char mode);
	void removeMode(char mode);
	bool hasMode(char mode) const;

	// Envoi de messages
	void broadcastMessage(const std::string& message, Client* sender = NULL);

private:
	std::string _name;
	std::string _topic;
	std::set<Client*> _clients;
	std::set<Client*> _operators;
	std::set<char> _modes;
};

#endif
