/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 13:47:41 by fprevot           #+#    #+#             */
/*   Updated: 2024/10/01 16:04:14 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "network/Channel.hpp"
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

	int getFd() const;
	std::string getNickname() const;
	std::string getUsername() const;
	std::string getRealname() const;
	std::string getHostname() const;
	ClientStatus getStatus() const;
	const std::set<Channel*>& getChannels() const;
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
