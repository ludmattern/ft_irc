/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 11:29:46 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/02 15:48:35 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/Command.hpp"
#include "network/Server.hpp"
#include "network/Client.hpp"
#include "network/Channel.hpp"
#include <sstream>
#include <vector>
#include <algorithm>

Quit::Quit() : Command() {}

Quit::~Quit() {}

void Quit::execute(Client& client, const std::vector<std::string>& params) 
{
	std::string quitMessage = "Client exited";
	if (!params.empty()) 
	{
		quitMessage = params[0];
	}

	std::string prefix = client.getPrefix();
	std::string message = ":" + prefix + " QUIT";
	if (!quitMessage.empty()) 
	{
		message += " :" + quitMessage;
	}
	//add chan leaving and broadcast msg on  chqn
	_server.closeClientConnection(client.getFd());
	
}