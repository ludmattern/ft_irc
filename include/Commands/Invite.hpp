/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 08:06:41 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/30 08:19:43 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INVITE_HPP
#define INVITE_HPP

#include "Command.hpp"

class Invite : public Command {
public:
    Invite();
    ~Invite();
    void execute(Server& server, Client& client, const std::vector<std::string>& params);
};

#endif
