/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 08:17:47 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/03 15:06:31 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/Command.hpp"
#include "network/Server.hpp"
#include "network/Client.hpp"
#include "replies.hpp"

User::User() {}
User::~User() {}

void User::execute(Client* client, const std::vector<std::string>& params)
{
	if (client->getStatus() == REGISTERED)
	{
		client->reply(ERR_ALREADYREGISTERED(client->getNickname()));
		return;
	}

	if (params.size() < 4)
	{
		client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "USER"));
		return;
	}

	std::string username = params[0];
	std::string realname = params[3];

	client->setUsername(username);
	client->setRealname(realname);

	tryRegister(client);
}
