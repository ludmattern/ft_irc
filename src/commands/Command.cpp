/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 17:26:22 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/01 17:29:49 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network/Server.hpp"
#include "commands/Command.hpp"

Command::Command() : _server(Server::getInstance()) {}

