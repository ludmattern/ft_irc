/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Info.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 16:20:33 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/05 16:27:49 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/Command.hpp"
#include "network/Server.hpp"
#include "replies.hpp"

Info::Info() : Command() {};
Info::~Info() {};

void Info::execute(Client* client, const std::vector<std::string>& params)
{
	if (params.size() > 0)
	{
		if (params[0] != _server.getName())
		{
			client->reply(ERR_NOSUCHSERVER(client->getNickname(), params[0]));
			return;
		}
	}

	client->reply(RPL_INFO(client->getNickname(), "This is the IRC server " + _server.getName()));
	client->reply(RPL_INFO(client->getNickname(), "Version 1.0.0"));
	client->reply(RPL_INFO(client->getNickname(), "Contact lmattern or fprevot for support"));
	client->reply(RPL_ENDOFINFO(client->getNickname()));
}
