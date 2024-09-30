/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 08:10:14 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/30 08:10:28 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PASS_HPP
#define PASS_HPP

#include "Command.hpp"

class Pass : public Command {
public:
    Pass();
    ~Pass();
    void execute(Server& server, Client& client, const std::vector<std::string>& params);
};

#endif
