/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 02:52:20 by fprevot           #+#    #+#             */
/*   Updated: 2024/10/06 02:52:21 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bot.h"
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <ctime>
#include <sstream>
#include <csignal>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char **argv)
{
	if (argc < 5)
	{
		std::cerr << "Usage: " << argv[0] << " <password> <server_ip> <port> <channel>\n";
		return 1;
	}
	struct sigaction sa;
	sa.sa_handler = handle_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		std::cerr << "Failed to set up SIGINT handler\n";
		return 1;
	}

	std::string password = argv[1];
	std::string server_ip = argv[2];
	int port = std::atoi(argv[3]);
	std::string nickname = "botNewman";
	std::string channel = "#" + std::string(argv[4]);

	std::srand(static_cast<unsigned int>(std::time(NULL)));

	std::string jokesFile = "jokes.txt"; 
	std::vector<std::string> jokes = loadJokes(jokesFile);
	if (jokes.empty())
		std::cerr << "No jokes found in file " << jokesFile << std::endl;
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		std::cerr << "Failed to create socket\n";
		return 1;
	}

	struct hostent* server = gethostbyname(server_ip.c_str());
	if (server == NULL) 
	{
		std::cerr << "Unknown server: " << server_ip << std::endl;
		close(sockfd);
		return 1;
	}

	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
	serv_addr.sin_port = htons(port);

	if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	{
		std::cerr << "Failed to connect to the server\n";
		close(sockfd);
		return 1;
	}

	std::string pass_command = "PASS " + password + "\r\n";
	std::string nick_command = "NICK " + nickname + "\r\n";
	std::string user_command = "USER " + nickname + " 0 * :Bot\r\n";
	std::string join_command = "JOIN " + channel + "\r\n";
	sendMessage(sockfd, pass_command);
	sendMessage(sockfd, nick_command);
	sendMessage(sockfd, user_command);
	sendMessage(sockfd, join_command);
	sendMessage(sockfd, "PRIVMSG " + channel + " :Available commands: !help, !date, !joke\r\n");
	run(sockfd, channel, jokes);
	return (0);
}
