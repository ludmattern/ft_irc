/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 08:12:14 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/30 08:12:38 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include "Command.hpp"

class User : public Command {
public:
    User();
    ~User();
    void execute(Server& server, Client& client, const std::vector<std::string>& params);
};

#endif
