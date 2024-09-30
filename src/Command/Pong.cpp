/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pong.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 08:17:14 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/30 08:24:25 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands/Pong.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

Pong::Pong() {}
Pong::~Pong() {}

void Pong::execute(Server& server, Client& client, const std::vector<std::string>& params)
{
	(void)client;
	(void)params;
	server.logToServer("PONG command received", "INFO");
}
