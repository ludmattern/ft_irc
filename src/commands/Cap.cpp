/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cap.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 13:42:41 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/05 14:13:49 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/Command.hpp"
#include "network/Server.hpp"
#include "network/Client.hpp"
#include "network/Channel.hpp"
#include "replies.hpp"

Cap::Cap() {}
Cap::~Cap() {}

void Cap::execute(Client* client, const std::vector<std::string>& params)
{
	if (params.empty())
	{
		client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "CAP"));
		return;
	}
	std::string subcommand = params[0];
	if (subcommand == "LS")
		client->write(RPL_CAP_LS);
	else if (subcommand == "END")
		client->write(RPL_CAP_END);
	else
		client->reply(ERR_UNKNOWNCOMMAND(client->getNickname(), "CAP"));
}
