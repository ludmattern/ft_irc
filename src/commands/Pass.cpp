/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 14:35:46 by fprevot           #+#    #+#             */
/*   Updated: 2024/10/02 14:25:28 by fprevot          ###   ########.fr       */
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
		client.reply(ERR_ALREADYREGISTRED(client.getNickname(), "PASS"));
	else if (params.empty())
		client.reply(ERR_NEEDMOREPARAMS(client.getNickname(), "PASS"));
	client.setPassword(params[0]);
	client.setStatus(AUTHENTICATED);
}