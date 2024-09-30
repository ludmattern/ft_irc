/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 08:11:54 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/30 08:12:11 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOPIC_HPP
#define TOPIC_HPP

#include "Command.hpp"

class Topic : public Command {
public:
    Topic();
    ~Topic();
    void execute(Server& server, Client& client, const std::vector<std::string>& params);
};

#endif
