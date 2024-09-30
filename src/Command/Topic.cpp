/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 08:17:38 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/30 08:24:31 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands/Topic.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

Topic::Topic() {}
Topic::~Topic() {}

void Topic::execute(Server& server, Client& client, const std::vector<std::string>& params)
{
	(void)client;
	(void)params;
	server.logToServer("TOPIC command received", "INFO");
}


