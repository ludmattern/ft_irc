/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 08:08:43 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/30 08:09:09 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NICK_HPP
#define NICK_HPP

#include "Command.hpp"

class Nick : public Command {
public:
    Nick();
    ~Nick();
    void execute(Server& server, Client& client, const std::vector<std::string>& params);
};

#endif