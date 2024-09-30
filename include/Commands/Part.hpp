/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 08:09:49 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/30 08:10:08 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PART_HPP
#define PART_HPP

#include "Command.hpp"

class Part : public Command {
public:
    Part();
    ~Part();
    void execute(Server& server, Client& client, const std::vector<std::string>& params);
};

#endif
