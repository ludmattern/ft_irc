/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 12:44:31 by fprevot           #+#    #+#             */
/*   Updated: 2024/10/03 01:17:22 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network/Server.hpp"

int main(int argc, char **argv)
{
	try {
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