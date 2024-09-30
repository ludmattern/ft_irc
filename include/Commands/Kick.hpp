/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 08:07:47 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/30 14:04:42 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KICK_HPP
#define KICK_HPP

#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"

class Kick : public Command {
public:
    Kick();
    ~Kick();
    void execute(Server& server, Client& client, const std::vector<std::string>& params);
};

#endif
