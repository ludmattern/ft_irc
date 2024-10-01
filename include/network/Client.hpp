/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 13:47:41 by fprevot           #+#    #+#             */
/*   Updated: 2024/10/01 14:43:43 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Channel.hpp"
#include <string>
#include <set>

enum ClientStatus {
	HANDSHAKE,
	AUTHENTICATED,
	REGISTERED,
	DISCONNECTED
};

class Client {
public:

	Client(int fd, const std::string &hostname);
	~Client();
	void reply(const std::string& reply);
	void write(const std::string& message) const;
	void joinChannel(Channel* channel, bool isOperator);
	void partChannel(Channel* channel);

	int Client::getFd() const;
	std::string Client::getNickname() const;
	std::string Client::getUsername() const;
	std::string Client::getRealname() const;
	std::string Client::getHostname() const;
	std::string Client::getPassword() const;
	ClientStatus Client::getStatus() const;
	const std::set<Channel*>& Client::getChannels() const;
	std::string getPrefix() const;

	void setNickname(const std::string& nickname);
	void setUsername(const std::string& username);
	void setRealname(const std::string& realname);
	void setStatus(ClientStatus status);

private:

    int _fd;
    std::string _nickname;
    std::string _username;
    std::string _realname;
    std::string _hostname;

	ClientStatus _status;
    std::set<Channel*> _channels;
};

#endif
