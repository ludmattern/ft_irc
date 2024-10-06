/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 12:45:14 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/06 16:29:18 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_HPP
#define REPLIES_HPP

#include <string>
#include <iostream>
#include <ctime>

/* Error messages (Numeric Replies) */

#define ERR_NOSUCHNICK(nick) "401 " + nick + " :No such nick/channel"
#define ERR_NOSUCHCHANNEL(nick, channel) "403 " + nick + " " + channel + " :No such channel"
#define ERR_CANNOTSENDTOCHAN(nick, channel) "404 " + nick + " " + channel + " :Cannot send to channel"
#define ERR_TOOMANYCHANNELS(nick, channel) "405 " + nick + " " + channel + " :You have joined too many channels"
#define ERR_UNKNOWNCOMMAND(nick, command) "421 " + nick + " " + command + " :Unknown command"
#define ERR_NONICKNAMEGIVEN() "431 :No nickname given"
#define ERR_ERRONEUSNICKNAME(nick) "432 " + nick + " :Erroneous nickname"
#define ERR_NICKNAMEINUSE(nick) "433 " + nick + " " + nick + " nickname is already in use"
#define ERR_USERNOTINCHANNEL(nick, target, channel) "441 " + nick + " " + target + " " + channel + " :They aren't on that channel"
#define ERR_NOTONCHANNEL(nick, channel) "442 " + nick + " " + channel + " :You're not on that channel"
#define ERR_USERONCHANNEL(nick, user, channel) "443 " + nick + " " + user + " " + channel + " :is already on channel"
#define ERR_NOTREGISTERED() "451 :You have not registered"
#define ERR_NEEDMOREPARAMS(nick, command) "461 " + nick + " " + command + " :Not enough parameters"
#define ERR_ALREADYREGISTERED(nick) "462 " + nick + " :You may not reregister"
#define ERR_PASSWDMISMATCH() "464 :Password incorrect"
#define ERR_CHANNELISFULL(nick, channel) "471 " + nick + " " + channel + " :Cannot join channel (+l)"
#define ERR_INVITEONLYCHAN(nick, channel) "473 " + nick + " " + channel + " :Cannot join channel (+i)"
#define ERR_BADCHANNELKEY(nick, channel) "475 " + nick + " " + channel + " :Cannot join channel (+k)"
#define ERR_CHANOPRIVSNEEDED(nick, channel) "482 " + nick + " " + channel + " :You're not channel operator"
#define ERR_UMODEUNKNOWNFLAG(nick) "501 " + nick + " :Unknown MODE flag"
#define ERR_INVALIDMODEPARAM(nick, channel, info) "696 " + nick + " " + channel + " :" + info
#define ERR_NORECIPIENT(nick, command) "411 " + nick + " " + command + " :No recipient given"
#define ERR_NOTEXTTOSEND(nick) "412 " + nick + " :No text to send"

/* Reply messages (Numeric Replies) */
#define RPL_WELCOME(nick) "001 " + nick + " :Welcome to the Internet Relay Network ft_irc " + nick
#define RPL_YOURHOST(nick, servername, version) "002 " + nick + " :Your host is " + servername + ", running version " + version
#define RPL_CREATED(nick, date) "003 " + nick + " :This server was created " + date
#define RPL_MYINFO(nick, servername, version, usermodes, channelmodes) "004 " + nick + " " + servername + " " + version + " " + usermodes + " " + channelmodes
#define RPL_TOPIC(nick, channel, topic) "332 " + nick + " " + channel + " :" + topic
#define RPL_NOTOPIC(nick, channel) "331 " + nick + " " + channel + " :No topic is set"
#define RPL_INVITING(nick, channel) "341 " + nick + " " + channel
#define RPL_NAMREPLY(nick, channel, userList) "353 " + nick + " = " + channel + " :" + userList
#define RPL_ENDOFNAMES(nick, channel) "366 " + nick + " " + channel + " :End of /NAMES list"
#define RPL_MOTD(nick, text) "372 " + nick + " :- " + text
#define RPL_MOTDSTART(nick, servername) "375 " + nick + " :- " + servername + " Message of the day - "
#define RPL_ENDOFMOTD(nick) "376 " + nick + " :End of /MOTD command"

/* Command responses (Non-numeric Replies) */
#define RPL_JOIN(prefix, channel) ":" + prefix + " JOIN :" + channel
#define RPL_PART(prefix, channel, message) ":" + prefix + " PART " + channel + " :" + message
#define RPL_QUIT(prefix, message) ":" + prefix + " QUIT :" + message
#define RPL_NICK(oldPrefix, newNick) ":" + oldPrefix + " NICK :" + newNick
#define RPL_PRIVMSG(prefix, target, message) ":" + prefix + " PRIVMSG " + target + " :" + message
#define RPL_NOTICE(prefix, target, message) ":" + prefix + " NOTICE " + target + " :" + message
#define RPL_PING(servername, token) "PING " + servername + " :" + token
#define RPL_PONG(servername, token) "PONG " + servername + " :" + token
#define RPL_CAP_LS ": CAP * LS: none"
#define RPL_CAP_END ": CAP END"
#define RPL_CHANNELMODEIS(nick, channel, mode) "324 " + nick + " " + channel + " " + mode
#define RPL_INFO(nick, info) "371 " + nick + " :" + info
#define RPL_ENDOFINFO(nick) "374 " + nick + " :End of /INFO list"
#define ERR_NOSUCHSERVER(nick, server) "402 " + nick + " " + server + " :No such server"
#define RPL_LISTSTART(nick) "321 " + nick + " Channel :Users  Name"
#define RPL_LIST(nick, channel, visible, topic) "322 " + nick + " " + channel + " " + visible + " :" + topic
#define RPL_LISTEND(nick) "323 " + nick + " :End of /LIST"
#define RPL_WHOREPLY(nick, channel, user, host, server, nickname, status, hopcount, realname) \
    "352 " + nick + " " + channel + " " + user + " " + host + " " + server + " " + nickname + " " + status + " :" + hopcount + " " + realname
#define RPL_ENDOFWHO(nick, mask) "315 " + nick + " " + mask + " :End of /WHO list"
#define ERR_NEEDMOREPARAMS(nick, command) "461 " + nick + " " + command + " :Not enough parameters"

/* Server Parameters */
#define MIN_PORT 1
#define MAX_PORT 65535
#define RESERVED_PORT 1024

/* Miscellaneous Constants */
#define GLOBAL_CHANNEL '#'
#define LOCAL_CHANNEL '&'
#define CRLF "\r\n"

/* Colors */
#define RESET       "\033[0m"
#define BLACK       "\033[30m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"

/* Text Styles */
#define BOLD        "\033[1m"
#define UNDERLINE   "\033[4m"

/* Logging Function */
static inline void log(const std::string& message)
{
    time_t      rawtime;
    struct tm   *timeinfo;
    char        buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
    std::string str(buffer);

    std::cout << "\033[0;34m[" << str << "]\033[0m ";
    std::cout << message << std::endl;
}

#endif
