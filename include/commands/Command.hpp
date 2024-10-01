/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 14:33:24 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/01 16:04:20 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "network/Client.hpp"
#include <vector>
#include <string>

class Server;
class Client;

class Command {
public:
	Command() {}
	virtual ~Command() {}
	virtual void execute(Client& client, const std::vector<std::string>& params) = 0;
};


class Pass : public Command {
public:
	Pass();
	~Pass();
	void execute(Client& client, const std::vector<std::string>& params);
};

#endif