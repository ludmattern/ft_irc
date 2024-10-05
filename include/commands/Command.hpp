/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 14:33:24 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/05 17:39:59 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "network/Server.hpp"
#include "network/Client.hpp"
#include <vector>
#include <string>

class Server;
class Client;

class Command {
public:
	Command();
	virtual ~Command() {}
	void tryRegister(Client* client);
	virtual void execute(Client* client, const std::vector<std::string>& params) = 0;
protected:
	Server& _server;
};

class Pass : public Command {
public:
	Pass();
	~Pass();
	void execute(Client* client, const std::vector<std::string>& params);
};

class User : public Command {
public:
	User();
	~User();
	void execute(Client* client, const std::vector<std::string>& params);
};

class Nick : public Command {
public:
	Nick();
	~Nick();
	void execute(Client* client, const std::vector<std::string>& params);
	bool isNicknameTaken(const std::string& nickname);
};

class Quit : public Command {
public:
	Quit();
	~Quit();
	void execute(Client* client, const std::vector<std::string>& params);
};

class Join : public Command {
public:
	Join();
	~Join();
	void execute(Client* client, const std::vector<std::string>& params);
};

class Topic : public Command {
public:
	Topic();
	~Topic();
	void execute(Client* client, const std::vector<std::string>& params);
};

class PrivMsg : public Command {
public:
	PrivMsg();
	~PrivMsg();
	void execute(Client* client, const std::vector<std::string>& params);
};

class Notice : public Command {
public:
	Notice();
	~Notice();
	void execute(Client* client, const std::vector<std::string>& params);
};

class Invite : public Command {
public:
	Invite();
	~Invite();
	void execute(Client* client, const std::vector<std::string>& params);
};

class Mode : public Command {
public:
	Mode();
	~Mode();
	void execute(Client* client, const std::vector<std::string>& params);
};

class Kick : public Command {
public:
	Kick();
	~Kick();
	void execute(Client* client, const std::vector<std::string>& params);
};
class Part : public Command {
public:
	Part();
	~Part();
	void execute(Client* client, const std::vector<std::string>& params);
};

class Cap : public Command {
public:
	Cap();
	~Cap();
	void execute(Client* client, const std::vector<std::string>& params);
};

class Info : public Command {
public:
	Info();
	~Info();
	void execute(Client* client, const std::vector<std::string>& params);
};

class List : public Command {
public:
	List();
	~List();
	void execute(Client* client, const std::vector<std::string>& params);
};

class Who : public Command {
public:
	Who();
	~Who();
	void execute(Client* client, const std::vector<std::string>& params);
};

#endif
