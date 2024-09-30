/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 08:17:02 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/30 08:30:30 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands/Pass.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

Pass::Pass() {}
Pass::~Pass() {}

void Pass::execute(Server& server, Client& client, const std::vector<std::string>& params) 
{
	if (client.isRegistered())
		server.sendError(&client, ERR_ALREADYREGISTRED, "", "You may not reregister");
	else if (params.empty())
		server.sendError(&client, ERR_NEEDMOREPARAMS, "PASS", "Not enough parameters");
	else
		client.setPassword(params[0]);
}

