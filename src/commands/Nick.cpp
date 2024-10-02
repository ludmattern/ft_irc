/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 08:16:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/03 01:17:06 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/Command.hpp"
#include "replies.hpp"

Nick::Nick() : Command() {}
Nick::~Nick() {}

void Nick::execute(Client& client, const std::vector<std::string>& params)
{
	if (params.empty())
	{
		client.reply(ERR_NEEDMOREPARAMS(client.getNickname(), "NICK"));
		return;
	}

	std::string newNickname = params[0];


	if (isNicknameTaken(newNickname))
	{
		client.reply(ERR_NICKNAMEINUSE(newNickname));
		return;
	}
	else
	{
		client.setNickname(newNickname);
		tryRegister(client);
	}
}



bool Nick::isNicknameTaken(const std::string& nickname)
{
	std::vector<Client*> clients = _server.getClients();
	for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if ((*it)->getNickname() == nickname)
			return true;
	}
	return false;
}
