/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 08:07:12 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/30 08:07:42 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JOIN_HPP
#define JOIN_HPP

#include "Command.hpp"

class Join : public Command {
public:
    Join();
    ~Join();
    void execute(Server& server, Client& client, const std::vector<std::string>& params);
};

#endif
