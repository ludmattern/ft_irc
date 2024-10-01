/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 13:47:41 by fprevot           #+#    #+#             */
/*   Updated: 2024/10/01 15:00:30 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "network/Channel.hpp"
#include <string>
#include <set>
#include <poll.h>

class Client {
public:
	void reply(int code, const std::string& message);
	void write(const std::string& message);
	void joinChannel(Channel* channel);
	void partChannel(Channel* channel);
    Client(int fd, std::string hostname);

private:

    struct pollfd _fd;
    std::string _nickname;
    std::string _username;
    std::string _realname;
    std::string _password;
    bool _isAuthenticated;

    std::set<Channel*> _channels;
};

#endif
