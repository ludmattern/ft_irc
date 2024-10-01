/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 08:20:05 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/01 15:57:05 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <sstream>
#include <string>

#include <vector>
#include <map>

class Server;

#include "commands/Command.hpp"

class Parser
{
    public:
        std::map<int, std::string> _clientInputBuffers;

        Parser();
        ~Parser();
        bool extractCommand(int client_fd, std::string& command);
        void processClientCommand(Client *client, const std::string &commandLine);
        void executeCommand(const std::string& commandName, Client& client, const std::vector<std::string>& params);
    private:

        std::map<std::string, Command*> _commands;

};

#endif