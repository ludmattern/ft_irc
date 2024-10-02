/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivMsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 11:29:45 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/02 11:29:46 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network/Server.hpp"
#include "network/Client.hpp"
#include "network/Channel.hpp"
#include "commands/Command.hpp"

PrivMsg::PrivMsg() {}

PrivMsg::~PrivMsg() {}

void PrivMsg::execute(Client& client, const std::vector<std::string>& params)
{
	(void)client;
	(void)params;
	std::cout << "Privmsg triggered\n";
}