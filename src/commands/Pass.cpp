/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 14:35:46 by fprevot           #+#    #+#             */
/*   Updated: 2024/10/03 01:17:11 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network/Server.hpp"
#include "network/Client.hpp"
#include "network/Channel.hpp"
#include "commands/Command.hpp"
#include "replies.hpp"

Pass::Pass() : Command() {}

Pass::~Pass() {}

void Pass::execute(Client& client, const std::vector<std::string>& params)
{
	if (client.getStatus() == REGISTERED)
	{
		client.reply(ERR_ALREADYREGISTERED(client.getNickname()));
		return;
	}
	if (params.empty())
	{
		client.reply(ERR_NEEDMOREPARAMS(client.getNickname(), "PASS"));
		return;
	}
	client.setPassword(params[0]);
	if (client.getPass() != _server.getPassword())
	{
		client.reply(ERR_PASSWDMISMATCH(client.getNickname()));
		return;
	}
	client.setAuthenticated(true);
	tryRegister(client);
}
