/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 16:00:24 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/30 08:32:54 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands/Notice.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include <sstream>
#include <vector>
#include <algorithm>

Notice::Notice() {}

Notice::~Notice() {}

void Notice::execute(Server& server, Client& client, const std::vector<std::string>& params) {
	if (!client.isRegistered()) {
		return;
	}

	if (params.size() < 2) {
		return;
	}

	std::string target = params[0];
	std::string message = params[1];

	for (size_t i = 2; i < params.size(); ++i) {
		message += " " + params[i];
	}

	if (!message.empty() && message[0] == ':') {
		message = message.substr(1);
	}

	std::string fullMessage = ":" + client.getPrefix() + " NOTICE " + target + " :" + message + "\r\n";

	if (target[0] == GLOBAL_CHANNEL || target[0] == LOCAL_CHANNEL) {
		Channel* channel = server.getChannelByName(target);
		if (channel) {
			const std::set<Client*>& clients = channel->getClients();
			for (std::set<Client*>::const_iterator it = clients.begin(); it != clients.end(); ++it) {
				if (*it != &client) {
					server.sendRawMessageToClient(*it, fullMessage);
				}
			}
		}
	} else {
		Client* targetClient = server.getClientByNickname(target);
		if (targetClient) {
			server.sendRawMessageToClient(targetClient, fullMessage);
		}
	}
}
