/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 08:10:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/30 08:10:48 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PING_HPP
#define PING_HPP

#include "Command.hpp"

class Ping : public Command {
public:
    Ping();
    ~Ping();
    void execute(Server& server, Client& client, const std::vector<std::string>& params);
};

#endif
