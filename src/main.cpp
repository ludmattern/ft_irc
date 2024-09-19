/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 13:47:24 by fprevot           #+#    #+#             */
/*   Updated: 2024/09/19 14:38:38 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return (EXIT_FAILURE);
	}

	int port = atoi(argv[1]);
	std::string password = argv[2];
	Server server(port, password);
	server.run();

	return (EXIT_SUCCESS);
}
