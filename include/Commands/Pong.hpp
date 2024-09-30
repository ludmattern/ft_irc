/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pong.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 08:10:51 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/30 08:11:09 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PONG_HPP
#define PONG_HPP

#include "Command.hpp"

class Pong : public Command {
public:
    Pong();
    ~Pong();
    void execute(Server& server, Client& client, const std::vector<std::string>& params);
};

#endif
