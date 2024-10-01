/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 08:20:05 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/01 15:00:24 by fprevot          ###   ########.fr       */
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

        Parser();
        ~Parser();
        void executeCommand(const std::string& commandName, Client& client, const std::vector<std::string>& params);
    private:

        std::map<std::string, Command*> _commands;

};

#endif