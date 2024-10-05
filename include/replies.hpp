/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 12:45:14 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/05 15:35:04 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_HPP
#define REPLIES_HPP

#include <string>
#include <iostream>
#include <ctime>

// **Error messages (Numeric Replies)**

// 401 ERR_NOSUCHNICK "<nick> :No such nick/channel"
#define ERR_NOSUCHNICK(nick) "401 " + nick + " :No such nick/channel"

// 403 ERR_NOSUCHCHANNEL "<channel> :No such channel"
#define ERR_NOSUCHCHANNEL(nick, channel) "403 " + nick + " " + channel + " :No such channel"

// 404 ERR_CANNOTSENDTOCHAN "<channel> :Cannot send to channel"
#define ERR_CANNOTSENDTOCHAN(nick, channel) "404 " + nick + " " + channel + " :Cannot send to channel"

// 405 ERR_TOOMANYCHANNELS "<channel> :You have joined too many channels"
#define ERR_TOOMANYCHANNELS(nick, channel) "405 " + nick + " " + channel + " :You have joined too many channels"

// 421 ERR_UNKNOWNCOMMAND "<command> :Unknown command"
#define ERR_UNKNOWNCOMMAND(nick, command) "421 " + nick + " " + command + " :Unknown command"

// 431 ERR_NONICKNAMEGIVEN ":No nickname given"
#define ERR_NONICKNAMEGIVEN() "431 :No nickname given"

// 432 ERR_ERRONEUSNICKNAME "<nick> :Erroneous nickname"
#define ERR_ERRONEUSNICKNAME(nick) "432 " + nick + " :Erroneous nickname"

// 433 ERR_NICKNAMEINUSE "<nick> :Nickname is already in use"
#define ERR_NICKNAMEINUSE(nick) "433 " + nick + " :Nickname is already in use"

// 441 ERR_USERNOTINCHANNEL "<nick> <channel> :They aren't on that channel"
#define ERR_USERNOTINCHANNEL(nick, target, channel) "441 " + nick + " " + target + " " + channel + " :They aren't on that channel"

// 442 ERR_NOTONCHANNEL "<channel> :You're not on that channel"
#define ERR_NOTONCHANNEL(nick, channel) "442 " + nick + " " + channel + " :You're not on that channel"

// 443 ERR_USERONCHANNEL "<user> <channel> :is already on channel"
#define ERR_USERONCHANNEL(nick, user, channel) "443 " + nick + " " + user + " " + channel + " :is already on channel"

// 451 ERR_NOTREGISTERED ":You have not registered"
#define ERR_NOTREGISTERED() "451 :You have not registered"

// 461 ERR_NEEDMOREPARAMS "<command> :Not enough parameters"
#define ERR_NEEDMOREPARAMS(nick, command) "461 " + nick + " " + command + " :Not enough parameters"

// 462 ERR_ALREADYREGISTERED ":You may not reregister"
#define ERR_ALREADYREGISTERED(nick) "462 " + nick + " :You may not reregister"

// 464 ERR_PASSWDMISMATCH ":Password incorrect"
#define ERR_PASSWDMISMATCH() "464 :Password incorrect"

// 471 ERR_CHANNELISFULL "<channel> :Cannot join channel (+l)"
#define ERR_CHANNELISFULL(nick, channel) "471 " + nick + " " + channel + " :Cannot join channel (+l)"

// 473 ERR_INVITEONLYCHAN "<channel> :Cannot join channel (+i)"
#define ERR_INVITEONLYCHAN(nick, channel) "473 " + nick + " " + channel + " :Cannot join channel (+i)"

// 475 ERR_BADCHANNELKEY "<channel> :Cannot join channel (+k)"
#define ERR_BADCHANNELKEY(nick, channel) "475 " + nick + " " + channel + " :Cannot join channel (+k)"

// 482 ERR_CHANOPRIVSNEEDED "<channel> :You're not channel operator"
#define ERR_CHANOPRIVSNEEDED(nick, channel) "482 " + nick + " " + channel + " :You're not channel operator"

// 501 ERR_UMODEUNKNOWNFLAG ":Unknown MODE flag"
#define ERR_UMODEUNKNOWNFLAG(nick) "501 " + nick + " :Unknown MODE flag"

// **Reply messages (Numeric Replies)**

// 001 RPL_WELCOME "<nick> :Welcome to the Internet Relay Network <nick>!<user>@<host>"
#define RPL_WELCOME(nick) "001 " + nick + " :Welcome to the Internet Relay Network ft_irc " + nick

// 002 RPL_YOURHOST "<nick> :Your host is <servername>, running version <ver>"
#define RPL_YOURHOST(nick, servername, version) "002 " + nick + " :Your host is " + servername + ", running version " + version

// 003 RPL_CREATED "<nick> :This server was created <date>"
#define RPL_CREATED(nick, date) "003 " + nick + " :This server was created " + date

// 004 RPL_MYINFO "<nick> <servername> <version> <available user modes> <available channel modes>"
#define RPL_MYINFO(nick, servername, version, usermodes, channelmodes) "004 " + nick + " " + servername + " " + version + " " + usermodes + " " + channelmodes

// 332 RPL_TOPIC "<nick> <channel> :<topic>"
#define RPL_TOPIC(nick, channel, topic) "332 " + nick + " " + channel + " :" + topic

// 331 RPL_NOTOPIC "<nick> <channel> :No topic is set"
#define RPL_NOTOPIC(nick, channel) "331 " + nick + " " + channel + " :No topic is set"

// 341 RPL_INVITING "<nick> <channel>"
#define RPL_INVITING(nick, channel) "341 " + nick + " " + channel

// 353 RPL_NAMREPLY "<nick> = <channel> :[[@|+]<nick> [[@|+]<nick> [...]]]"
#define RPL_NAMREPLY(nick, channel, userList) "353 " + nick + " = " + channel + " :" + userList

// 366 RPL_ENDOFNAMES "<nick> <channel> :End of /NAMES list"
#define RPL_ENDOFNAMES(nick, channel) "366 " + nick + " " + channel + " :End of /NAMES list"

// 372 RPL_MOTD "<nick> :- <text>"
#define RPL_MOTD(nick, text) "372 " + nick + " :- " + text

// 375 RPL_MOTDSTART "<nick> :- <server> Message of the day - "
#define RPL_MOTDSTART(nick, servername) "375 " + nick + " :- " + servername + " Message of the day - "

// 376 RPL_ENDOFMOTD "<nick> :End of /MOTD command"
#define RPL_ENDOFMOTD(nick) "376 " + nick + " :End of /MOTD command"

// **Command responses (Non-numeric Replies)**

// JOIN message
#define RPL_JOIN(prefix, channel) ":" + prefix + " JOIN :" + channel

// PART message
#define RPL_PART(prefix, channel, message) ":" + prefix + " PART " + channel + " :" + message

// QUIT message
#define RPL_QUIT(prefix, message) ":" + prefix + " QUIT :" + message

// NICK message
#define RPL_NICK(oldPrefix, newNick) ":" + oldPrefix + " NICK :" + newNick

// PRIVMSG message
#define RPL_PRIVMSG(prefix, target, message) ":" + prefix + " PRIVMSG " + target + " :" + message

// NOTICE message
#define RPL_NOTICE(prefix, target, message) ":" + prefix + " NOTICE " + target + " :" + message

// PING message
#define RPL_PING(servername, token) "PING " + servername + " :" + token

// PONG message
#define RPL_PONG(servername, token) "PONG " + servername + " :" + token

//CAP LS message
#define RPL_CAP_LS ": CAP * LS: none"
#define RPL_CAP_END ": CAP END"

//MODE 324 reply
#define RPL_CHANNELMODEIS(nick, channel, mode) "324 " + nick + " " + channel + " +" + mode


// **Server Parameters**
#define MIN_PORT 1
#define MAX_PORT 65535
#define RESERVED_PORT 1024

// **Miscellaneous Constants**
#define GLOBAL_CHANNEL '#'
#define LOCAL_CHANNEL '&'
#define CRLF "\r\n"

// **Colors**
#define RESET       "\033[0m"
#define BLACK       "\033[30m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"

// **Text Styles**
#define BOLD        "\033[1m"
#define UNDERLINE   "\033[4m"

// **Logging Function**
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
