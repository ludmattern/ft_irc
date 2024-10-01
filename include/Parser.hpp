/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 08:20:05 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/01 11:24:02 by lmattern         ###   ########.fr       */
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

#include "Command.hpp"

class Parser
{
    public:

        Parser();
        ~Parser();
        void executeCommand(const std::string& commandName, Server& server, Client& client, const std::vector<std::string>& params);
    private:

        std::map<std::string, Command*> _commands;

};

#endif