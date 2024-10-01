/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 10:43:51 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/01 16:04:16 by fprevot          ###   ########.fr       */
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

	void broadcast(const std::string& message);
	void broadcast(const std::string& message, Client* sender);
	
	void addClient(Client* client, bool isOperator);
	void removeClient(Client* client);

private:
	std::string _name;
	std::string _topic;
	std::set<char> _modes;
	std::map<Client*, bool> _clients;
};

#endif
