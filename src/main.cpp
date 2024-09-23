/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 15:32:29 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/23 16:51:19 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <csignal>

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        return EXIT_FAILURE;
    }

    int port = atoi(argv[1]);
    std::string password = argv[2];
    try
    {
        Server server(port, password);
       server.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << '\n';
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}
