/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 18:13:43 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/27 00:17:53 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Command.hpp"
#include<algorithm>


Command* CommandFactory::createCommand(const std::string& commandName) {
    if (commandName == "PASS") {
        return new Pass();
    } else if (commandName == "NICK") {
        return new Nick();
    } else if (commandName == "USER") {
        return new User();
    } else if (commandName == "JOIN") {
        return new Join();
    } else if (commandName == "PRIVMSG") {
        return new PrivMsg();
    } else if (commandName == "QUIT") {
        return new Quit();
    } else if (commandName == "NOTICE") {
        return new Notice();
    } else if (commandName == "PART") {
        return new Part();
    } else if (commandName == "TOPIC") {
        return new Topic();
    } else if (commandName == "PING") {
        return new Ping();
    } else if (commandName == "PONG") {
        return new Pong();
    } else if (commandName == "KICK") {
        return new Kick();
    } else if (commandName == "INVITE") {
        return new Invite();
    } else {
        return NULL;
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
            sendError(client, "421", "", "Malformed command");
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

    // Convertir en majuscules
    std::transform(commandName.begin(), commandName.end(), commandName.begin(), ::toupper);

    // Utiliser CommandFactory pour créer la commande
    Command* cmd = CommandFactory::createCommand(commandName);
    if (cmd) 
    {
        cmd->execute(*this, *client, params);
        delete cmd;
    }
    else 
    {
		sendError(client, ERR_UNKNOWNCOMMAND, commandName, "Unknown command");
    }
}
