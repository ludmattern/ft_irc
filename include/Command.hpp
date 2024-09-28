/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 14:33:24 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/28 14:37:22 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <vector>
#include <string>

class Server;
class Client;

class Command {
public:
    Command() {}
    virtual ~Command() {}
    virtual void execute(Server& server, Client& client, const std::vector<std::string>& params) = 0;
};


class Pass : public Command {
public:
    Pass();
    ~Pass();
    void execute(Server& server, Client& client, const std::vector<std::string>& params);
};

class Nick : public Command {
public:
    Nick();
    ~Nick();
    void execute(Server& server, Client& client, const std::vector<std::string>& params);
};

class User : public Command {
public:
    User();
    ~User();
    void execute(Server& server, Client& client, const std::vector<std::string>& params);
};

class Join : public Command {
public:
    Join();
    ~Join();
    void execute(Server& server, Client& client, const std::vector<std::string>& params);
};

class PrivMsg : public Command {
public:
    PrivMsg();
    ~PrivMsg();
    void execute(Server& server, Client& client, const std::vector<std::string>& params);
};

class Quit : public Command {
public:
    Quit();
    ~Quit();
    void execute(Server& server, Client& client, const std::vector<std::string>& params);
};

class Notice : public Command {
public:
    Notice();
    ~Notice();
    void execute(Server& server, Client& client, const std::vector<std::string>& params);
};

class Part : public Command {
public:
    Part();
    ~Part();
    void execute(Server& server, Client& client, const std::vector<std::string>& params);
};

class Topic : public Command {
public:
    Topic();
    ~Topic();
    void execute(Server& server, Client& client, const std::vector<std::string>& params);
};

class Ping : public Command {
public:
    Ping();
    ~Ping();
    void execute(Server& server, Client& client, const std::vector<std::string>& params);
};

class Pong : public Command {
public:
    Pong();
    ~Pong();
    void execute(Server& server, Client& client, const std::vector<std::string>& params);
};

class Kick : public Command {
public:
    Kick();
    ~Kick();
    void execute(Server& server, Client& client, const std::vector<std::string>& params);
};

class Invite : public Command {
public:
    Invite();
    ~Invite();
    void execute(Server& server, Client& client, const std::vector<std::string>& params);
};

class Mode : public Command {
public:
    Mode() {}
    virtual ~Mode() {}
    virtual void execute(Server& server, Client& client, const std::vector<std::string>& params);
};

class CommandFactory {
public:
    static Command* createCommand(const std::string& commandName);
};

#endif
