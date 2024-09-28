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
    private:

        std::map<std::string, Command*> _commands;
    public:

        Parser();
        ~Parser();
        void executeCommand(const std::string& commandName, Server& server, Client& client, const std::vector<std::string>& params);

};

#endif