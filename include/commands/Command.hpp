/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 14:33:24 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/01 17:30:36 by lmattern         ###   ########.fr       */
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
    virtual void execute(Client& client, const std::vector<std::string>& params) = 0;
protected:
	Server& _server;
};


class Pass : public Command {
public:
	Pass();
	~Pass();
	void execute(Client& client, const std::vector<std::string>& params);
};

class User : public Command {
public:
	User();
	~User();
	void execute(Client& client, const std::vector<std::string>& params);
};

class Nick : public Command {
public:
	Nick();
	~Nick();
	void execute(Client& client, const std::vector<std::string>& params);
	bool isNicknameTaken(const std::string& nickname);
};


#endif