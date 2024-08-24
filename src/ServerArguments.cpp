/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerArguments.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 17:33:37 by lmattern          #+#    #+#             */
/*   Updated: 2024/08/21 17:42:59 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <climits>

#define MIN_PORT 1
#define MAX_PORT 65535
#define RESERVED_PORT 1024

std::pair<int, std::string> Server::parseArguments(int argc, char **argv)
{
    if (argc != 3)
        throw std::invalid_argument("Usage: " + std::string(argv[0]) + " <listening port> <connection password>");

    char *endptr;
    long port = std::strtol(argv[1], &endptr, 10);

    if (*endptr != '\0' || endptr == argv[1])
        throw std::invalid_argument("Error: Port must be a valid integer.");

    if (port < MIN_PORT || port > MAX_PORT)
        throw std::invalid_argument("Error: Port must be a positive integer between 1 and 65535.");

    if (port < RESERVED_PORT)
        std::cerr << "Warning: Ports below 1024 are typically reserved for system services." << std::endl;

    std::string password = argv[2];
    return (std::make_pair(static_cast<int>(port), password));
}
