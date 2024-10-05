/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 14:27:00 by fprevot           #+#    #+#             */
/*   Updated: 2024/10/05 17:50:51 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "network/Server.hpp"
#include "commands/Command.hpp"
#include "replies.hpp"
#include <iostream>
#include <algorithm>

Parser::Parser()
{
	_commands["PASS"] = new Pass();
	_commands["NICK"] = new Nick();
	_commands["USER"] = new User();
	_commands["QUIT"] = new Quit();
	_commands["JOIN"] = new Join();
	_commands["PART"] = new Part();
	_commands["KICK"] = new Kick();
	_commands["MODE"] = new Mode();
	_commands["PRIVMSG"] = new PrivMsg();
	_commands["NOTICE"] = new Notice();
	_commands["TOPIC"] = new Topic();
	_commands["INVITE"] = new Invite();
	_commands["CAP"] = new Cap();
	_commands["INFO"] = new Info();
	_commands["LIST"] = new List();
	_commands["WHO"] = new Who();
}

Parser::~Parser()
{
	for (std::map<std::string, Command *>::iterator it = _commands.begin(); it != _commands.end(); ++it)
		delete it->second;
	_commands.clear();
}

void Parser::executeCommand(const std::string &commandName, Client* client, const std::vector<std::string> &params)
{
	std::map<std::string, Command *>::iterator it = _commands.find(commandName);
	if (it != _commands.end())
		it->second->execute (client, params);
	else
		client->reply(ERR_UNKNOWNCOMMAND(client->getNickname(), commandName));
}

void Parser::processClientCommand(Client *client, const std::string &commandLine)
{
	std::string line = commandLine;
	std::string prefix;

	if (!line.empty() && line[0] == ':')
	{
		size_t pos = line.find(' ');
		if (pos != std::string::npos)
		{
			prefix = line.substr(1, pos - 1);
			line = line.substr(pos + 1);
		}
		else
		{
			client->reply(ERR_UNKNOWNCOMMAND(client->getNickname(), line));
			return;
		}
	}

	std::istringstream iss(line);
	std::string commandName;
	iss >> commandName;

	std::vector<std::string> params;
	std::string param;
	while (iss >> param)
	{
		if (param[0] == ':')
		{
			std::string rest;
			getline(iss, rest);
			if (!rest.empty())
				param += rest;
			param = param.substr(1);
			params.push_back(param);
			break;
		}
		else
			params.push_back(param);
	}

	std::transform(commandName.begin(), commandName.end(), commandName.begin(), ::toupper);
	executeCommand(commandName, client, params);
}