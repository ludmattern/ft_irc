/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 14:35:46 by fprevot           #+#    #+#             */
/*   Updated: 2024/10/01 16:04:10 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "network/Server.hpp"
#include "network/Client.hpp"
#include "network/Channel.hpp"
#include "commands/Command.hpp"

Pass::Pass() {}

Pass::~Pass() {}

void Pass::execute(Client& client, const std::vector<std::string>& params)
{
	(void)client;
	(void)params;
	std::cout << "WORKING\n";
}