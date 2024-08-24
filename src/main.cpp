/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 18:21:42 by lmattern          #+#    #+#             */
/*   Updated: 2024/08/21 17:42:52 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>
#include <csignal>

int main(int argc, char *argv[])
{
	try
	{
		std::pair<int, std::string> args = Server::parseArguments(argc, argv);
		int port = args.first;
		std::string password = args.second;

		Server ircServer(port, password);
		std::cout << "---- SERVER ----" << std::endl;

		signal(SIGINT, Server::ProcessSignal);
		signal(SIGQUIT, Server::ProcessSignal);
		ircServer.start();
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return (ERROR);
	}

	std::cout << "The Server Closed!" << std::endl;
	return (SUCCESS);
}
