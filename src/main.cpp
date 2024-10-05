/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 12:44:31 by fprevot           #+#    #+#             */
/*   Updated: 2024/10/05 14:26:12 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network/Server.hpp"

int main(int argc, char **argv)
{
	try
	{
		Server& server = Server::getInstance();
		server.init(argc, argv);
		server.run();
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error: "  << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}