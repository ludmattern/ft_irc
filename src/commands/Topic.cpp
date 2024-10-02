/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 11:29:48 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/02 11:29:49 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/Command.hpp"
#include "network/Server.hpp"
#include "network/Client.hpp"
#include "network/Channel.hpp"


Topic::Topic() {}

Topic::~Topic() {}

void Topic::execute(Client& client, const std::vector<std::string>& params) 
{
    (void)client;
    (void)params;
    std::cout << "Topic triggered\n";
}