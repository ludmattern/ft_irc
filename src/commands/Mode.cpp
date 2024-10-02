/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 11:29:38 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/03 01:17:05 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network/Server.hpp"
#include "network/Client.hpp"
#include "network/Channel.hpp"
#include "commands/Command.hpp"

Mode::Mode() {}
Mode::~Mode() {}

void Mode::execute(Client& client, const std::vector<std::string>& params)
{
	(void)client;
	(void)params;
	std::cout << "Mode triggered\n";
}