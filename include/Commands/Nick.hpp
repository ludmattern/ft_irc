/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 08:08:43 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/30 14:04:55 by fprevot          ###   ########.fr       */
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