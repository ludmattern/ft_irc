/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 13:47:41 by fprevot           #+#    #+#             */
/*   Updated: 2024/10/06 16:20:31 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "network/Channel.hpp"
#include <string>
#include <set>

class Channel;

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
	std::string getPrefix() const;
	std::string getPass() const;
	ClientStatus getStatus() const;
	const std::set<Channel*>& getChannels() const;

	void setNickname(const std::string& nickname);
	void setUsername(const std::string& username);
	void setRealname(const std::string& realname);
	void setStatus(ClientStatus status);
	void setPassword(const std::string& password);
	void setAuthenticated(bool value);

	bool isAuthenticated() const;

private:
	bool _authenticated;

	int _fd;
	std::string _nickname;
	std::string _username;
	std::string _realname;
	std::string _hostname;
	std::string _password;
	ClientStatus _status;
	std::set<Channel*> _channels;
};

#endif
