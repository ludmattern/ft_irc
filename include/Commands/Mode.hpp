/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 08:08:10 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/30 13:09:25 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODE_HPP
#define MODE_HPP

#include "Command.hpp"

class Mode : public Command {
public:
    Mode() {}
    ~Mode() {}
    void execute(Server& server, Client& client, const std::vector<std::string>& params);
};

#endif