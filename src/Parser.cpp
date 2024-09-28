/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 18:13:43 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/28 14:44:33 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Command.hpp"
#include "Parser.hpp"
#include<algorithm>


Parser::Parser() {
    _commands["PASS"] = new Pass();
    _commands["NICK"] = new Nick();
    _commands["USER"] = new User();
    _commands["QUIT"] = new Quit();
    _commands["PING"] = new Ping();
    _commands["PONG"] = new Pong();
    _commands["JOIN"] = new Join();
    _commands["PART"] = new Part();
    _commands["KICK"] = new Kick();
    _commands["MODE"] = new Mode();
    _commands["PRIVMSG"] = new PrivMsg();
    _commands["NOTICE"] = new Notice();
    _commands["TOPIC"] = new Topic();
    _commands["INVITE"] = new Invite();
}

Parser::~Parser()
{
    for (std::map<std::string, Command*>::iterator it = _commands.begin(); it != _commands.end(); ++it) {
        delete it->second;
    }
    _commands.clear();
}


void Parser::executeCommand(const std::string& commandName, Server& server, Client& client, const std::vector<std::string>& params) {
    std::map<std::string, Command*>::iterator it = _commands.find(commandName);
    if (it != _commands.end()) {
        it->second->execute(server, client, params);
    } else {
        server.sendError(&client, "421", commandName, "Unknown command");
    }
}

void Server::processClientCommand(Client* client, const std::string& commandLine) 
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
			sendError(client, ERR_UNKNOWNCOMMAND, "", "Malformed command");
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
    _commandHandler->executeCommand(commandName, *this, *client, params);

}
