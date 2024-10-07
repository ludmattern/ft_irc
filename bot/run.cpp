/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 02:53:43 by fprevot           #+#    #+#             */
/*   Updated: 2024/10/07 13:09:39 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bot.h"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include <sstream>
#include <csignal>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

volatile sig_atomic_t stop_requested = 0;

void handle_sigint(int signum)
{
	(void)signum;
	stop_requested = 1;
}

void sendMessage(int sockfd, const std::string& message)
{
	if (send(sockfd, message.c_str(), message.length(), 0) < 0)
		std::cerr << "Failed to send message: " << message << std::endl;
}

void sendQuitCommand(int sockfd, const std::string& reason)
{
	std::string quit_command = "QUIT :" + reason + "\r\n";
	sendMessage(sockfd, quit_command);
	std::cout << "QUIT command sent. Exiting the bot...\n";
	close(sockfd);
}

void handleOperatorMode(int sockfd, const std::string& channel)
{
	std::cout << "bot is oper\n";
	
	std::string opermsg = "PRIVMSG " + channel + " :FUCK YOU ALL, i go sleep now, try to awake me and ou will see...\r\n";
	sendMessage(sockfd, opermsg);
	std::string topic_command = "TOPIC " + channel + " Fuck you all NERDS\r\n";
	sendMessage(sockfd, topic_command);
	std::string mode_command = "MODE " + channel + " +k fuck\r\n";
	sendMessage(sockfd, mode_command);
}

std::string parseNickname(const std::string& message) 
{
	if (message.empty() || message[0] != ':')
		return "";

	size_t exclPos = message.find('!');
	if (exclPos == std::string::npos)
		return "";

	return message.substr(1, exclPos - 1);
}

void processMessage(int sockfd, const std::string& msg, const std::string& channel, const std::vector<std::string>& jokes)
{
	if (msg.find("!help") != std::string::npos) 
	{
		std::string reply = "PRIVMSG " + channel + " :Available commands: !help, !date, !joke\r\n";
		sendMessage(sockfd, reply);
	}
	else if (msg.find("!date") != std::string::npos)
	{
		std::time_t now = std::time(NULL);
		std::tm* now_tm = std::localtime(&now);
		char date_time[100];
		std::strftime(date_time, sizeof(date_time), "%Y-%m-%d %H:%M:%S", now_tm);
		std::ostringstream reply_stream;
		reply_stream << "PRIVMSG " << channel << " :Current date and time: " << date_time << "\r\n";
		std::string reply = reply_stream.str();
		sendMessage(sockfd, reply);
	}
	else if (msg.find("!joke") != std::string::npos)
	{
		if (!jokes.empty())
		{
			int index = std::rand() % jokes.size();
			std::string joke = jokes[index];
			std::string reply = "PRIVMSG " + channel + " :" + joke + "\r\n";
			sendMessage(sockfd, reply);
		}
		else
		{
			std::string reply = "PRIVMSG " + channel + " :Sorry, nothing to say.\r\n";
			sendMessage(sockfd, reply);
		}
	}
}

bool handleKickEvent(int sockfd, const std::string& response, const std::string& channel, const std::string& nickname)
{
	if (response.find("KICK " + channel + " " + nickname) != std::string::npos)
	{
		std::cout << "The bot has been kicked from the channel!\n";
		sendQuitCommand(sockfd, "I was kicked");
		return (true);
	}
	return (false);
}

void kickUser(int sockfd, const std::string& channel, const std::string& nickname, const std::string& reason)
{
	std::string kickCommand = "KICK " + channel + " " + nickname + " :" + reason + "\r\n";
	if (nickname != "botNewman")
		sendMessage(sockfd, kickCommand);
}

bool handleServerResponse(int sockfd, const std::string& response, const std::string& channel, const std::string& nickname, const std::vector<std::string>& jokes)
{
	if (handleKickEvent(sockfd, response, channel, nickname))
		return (true);
	else if (response.find("PRIVMSG " + channel + " :") != std::string::npos)
	{
		size_t pos = response.find("PRIVMSG " + channel + " :") + ("PRIVMSG " + channel + " :").length();
		std::string msg = response.substr(pos);
		processMessage(sockfd, msg, channel, jokes);
	}
	return (false);
}

std::vector<std::string> loadJokes(const std::string& filename)
{
	std::vector<std::string> jokes;
	std::ifstream file(filename.c_str());
	if (!file.is_open())
	{
		std::cerr << "Cannot load file " << filename << std::endl;
		return (jokes);
	}
	std::string line;
	while (std::getline(file, line))
	{
		if (!line.empty())
			jokes.push_back(line);
	}
	file.close();
	return (jokes);
}

void run(int sockfd, const std::string& channel, const std::vector<std::string>& jokes)
{
	char buffer[512];
	std::string nickname = "botNewman";
	bool oper = false;

	while (true)
	{
		if (stop_requested)
		{
			sendQuitCommand(sockfd, "Bye!");
			break;
		}

		memset(buffer, 0, sizeof(buffer));
		int n = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
		if (n < 0)
		{
			if (errno == EINTR)
				continue;
			else
			{
				std::cerr << "Error receiving data from server\n";
				break;
			}
		}
		else if (n == 0)
		{
			std::cerr << "Server disconnected\n";
			break;
		}
		std::string response(buffer);
		std::cout << response << std::endl;
		if (response.find("MODE " + channel + " +o " + nickname) != std::string::npos)
		{
			handleOperatorMode(sockfd, channel);
			oper = true;
			continue;
		}
		if (response.find("KICK " + channel + " " + nickname) != std::string::npos)
		{
			handleKickEvent(sockfd, response, channel, nickname);
			break;
		}
		size_t privmsg_pos = response.find("PRIVMSG " + channel + " :");
		if (privmsg_pos != std::string::npos)
		{
			size_t msg_start = privmsg_pos + ("PRIVMSG " + channel + " :").length();
			std::string msg = response.substr(msg_start);
			std::string senderNick = parseNickname(response);
			if (oper && senderNick != nickname)
			{
				kickUser(sockfd, channel, senderNick, "fuck you, let me sleep.");
			}
			else
			{
				processMessage(sockfd, msg, channel, jokes);
			}
		}
	}
	close(sockfd);
}
