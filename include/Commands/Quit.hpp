/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 08:11:37 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/30 08:11:51 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUIT_HPP
#define QUIT_HPP

#include "Command.hpp"

class Quit : public Command {
public:
    Quit();
    ~Quit();
    void execute(Server& server, Client& client, const std::vector<std::string>& params);
};

#endif
