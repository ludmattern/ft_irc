/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 11:29:46 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/02 11:29:47 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/Command.hpp"
#include "network/Server.hpp"
#include "network/Client.hpp"
#include "network/Channel.hpp"
#include <sstream>
#include <vector>
#include <algorithm>

Quit::Quit() {}

Quit::~Quit() {}

void Quit::execute(Client& client, const std::vector<std::string>& params) 
{
    (void)client;
    (void)params;
    std::cout << "quit triggered\n";
}