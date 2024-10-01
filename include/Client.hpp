/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 13:47:41 by fprevot           #+#    #+#             */
/*   Updated: 2024/10/01 11:52:53 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Channel.hpp"
#include <string>
#include <set>
#include <poll.h>

class Client {
private:

    struct pollfd _fd;
    std::string _nickname;
    std::string _username;
    std::string _realname;
    std::string _password;
    bool _isAuthenticated;

    std::set<Channel*> _channels;

public:
	void reply(int code, const std::string& message);
	void write(const std::string& message);
};

#endif
