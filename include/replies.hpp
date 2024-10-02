/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 12:45:14 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/02 14:30:12 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_HPP
#define REPLIES_HPP

#include <string>
#include <iostream>
#include <ctime>

// Error messages
#define ERR_NOSUCHNICK(source, nickname)                "401 " + source + " " + nickname + " :No such nick/channel"
#define ERR_NOSUCHCHANNEL(source, channel)              "403 " + source + " " + channel + " :No such channel"
#define ERR_CANNOTSENDTOCHAN(source, channel)           "404 " + source + " " + channel + " :Cannot send to channel"
#define ERR_TOOMANYCHANNELS(source, channel)            "405 " + source + " " + channel + " :You have joined too many channels"
#define ERR_UNKNOWNCOMMAND(source, command)             "421 " + source + " " + command + " :Unknown command"
#define ERR_NONICKNAMEGIVEN(source)                     "431 " + source + " :Nickname not given"
#define ERR_NOUSERNAMEGIVEN(source)                     "432 " + source + " :No username given"
#define ERR_NICKNAMEINUSE(source)                       "433 " + source + " " + source  + " :Nickname is already in use"
#define ERR_USERNOTINCHANNEL(source, nickname, channel) "441 " + source + " " + nickname + " " + channel + " :They aren't on that channel"
#define ERR_NOTONCHANNEL(source, channel)               "442 " + source + " " + channel + " :You're not on that channel"
#define ERR_NOTREGISTERED(source)                       "451 " + source + " :You have not registered"
#define ERR_NEEDMOREPARAMS(source, command)             "461 " + source + " " + command + " :Not enough parameters"
#define ERR_ALREADYREGISTERED(source)                   "462 " + source + " :You may not register"
#define ERR_PASSWDMISMATCH(source)                      "464 " + source + " :Password is incorrect"
#define ERR_CHANNELISFULL(source, channel)              "471 " + source + " " + channel + " :Cannot join channel (+l)"
#define ERR_BADCHANNELKEY(source, channel)              "475 " + source + " " + channel + " :Cannot join channel (+k)"
#define ERR_CHANOPRIVSNEEDED(source, channel)           "482 " + source + " " + channel + " :You're not channel operator"
#define ERR_ALREADYREGISTRED(source, channel)           "462 " + source + " :You are alreqdy connected"

// replies
#define RPL_WELCOME(source)                             "001 " + source + " :Welcome " + source + " to the ft_irc network"
#define RPL_TOPIC(source, channel, topic)               "332 " + source + " " + channel + " :" + topic
#define RPL_NAMREPLY(source, channel, users)            "353 " + source + " = " + channel + " :" + users
#define RPL_ENDOFNAMES(source, channel)                 "366 " + source + " " + channel + " :End of /NAMES list."
#define RPL_NICK(source, message)                       ":" + source + " NICK :" + message
#define RPL_JOIN(source, channel)                       ":" + source + " JOIN :" + channel
#define RPL_PART(source, channel)                       ":" + source + " PART :" + channel
#define RPL_PING(source, token)                         ":" + source + " PONG :" + token
#define RPL_PRIVMSG(source, target, message)            ":" + source + " PRIVMSG " + target + " :" + message
#define RPL_NOTICE(source, target, message)             ":" + source + " NOTICE " + target + " :" + message
#define RPL_QUIT(source, message)                       ":" + source + " QUIT :Quit: " + message
#define RPL_KICK(source, channel, target, reason)       ":" + source + " KICK " + channel + " " + target + " :" + reason
#define RPL_MODE(source, channel, modes, args)          ":" + source + " MODE " + channel + " " + modes + " " + args

// Server Params
#define MIN_PORT 1
#define MAX_PORT 65535
#define RESERVED_PORT 1024

//Misc Constants
#define GLOBAL_CHANNEL '#'
#define LOCAL_CHANNEL '&'
#define CRLF "\r\n"

// Colors
#define RESET       "\033[0m"
#define BLACK       "\033[30m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"

// Text styles
#define BOLD        "\033[1m"
#define UNDERLINE   "\033[4m"
#define BRED		"\033[1;31m"

static inline void log(const std::string& message) 
{
	time_t      rawtime;
	struct tm   *timeinfo;
	char        buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
	std::string str(buffer);

	(void)message;
	std::cout << "\033[0;34m[" << str << "]\033[0m ";
	std::cout << message << std::endl;
}

#endif