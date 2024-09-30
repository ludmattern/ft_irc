/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Notice.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 08:09:16 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/30 08:09:45 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NOTICE_HPP
#define NOTICE_HPP

#include "Command.hpp"

class Notice : public Command {
public:
    Notice();
    ~Notice();
    void execute(Server& server, Client& client, const std::vector<std::string>& params);
};

#endif
