/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivMsg.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 08:11:15 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/30 08:11:34 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIVMSG_HPP
#define PRIVMSG_HPP

#include "Command.hpp"

class PrivMsg : public Command {
public:
    PrivMsg();
    ~PrivMsg();
    void execute(Server& server, Client& client, const std::vector<std::string>& params);
};

#endif
