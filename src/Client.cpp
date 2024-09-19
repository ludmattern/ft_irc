/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 14:01:39 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/19 14:03:58 by lmattern         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <sstream>

Client::Client(int fd, const std::string& ip)
    : _fd(fd), _ip(ip), _is_authenticated(false), _is_registered(false) {
}

Client::~Client() {
}

int Client::getFd() const { return _fd; }
const std::string& Client::getIp() const { return _ip; }
const std::string& Client::getNickname() const { return _nickname; }
const std::string& Client::getUsername() const { return _username; }
const std::string& Client::getRealname() const { return _realname; }
const std::string& Client::getPassword() const { return _password; }
const std::string& Client::getPrefix() const { return _prefix; }
bool Client::isAuthenticated() const { return _is_authenticated; }
bool Client::isRegistered() const { return _is_registered; }

void Client::setNickname(const std::string& nickname) {
    _nickname = nickname;
    updatePrefix();
}

void Client::setUsername(const std::string& username) {
    _username = username;
    updatePrefix();
}

void Client::setRealname(const std::string& realname) {
    _realname = realname;
}

void Client::setPassword(const std::string& password) {
    _password = password;
}

void Client::setAuthenticated(bool authenticated) {
    _is_authenticated = authenticated;
}

void Client::setRegistered(bool registered) {
    _is_registered = registered;
}

void Client::appendToInputBuffer(const std::string& data) {
    _input_buffer += data;
}

std::string& Client::getInputBuffer() {
    return _input_buffer;
}

void Client::joinChannel(const std::string& channel) {
    _channels.insert(channel);
}

void Client::leaveChannel(const std::string& channel) {
    _channels.erase(channel);
}

const std::set<std::string>& Client::getChannels() const {
    return _channels;
}

void Client::updatePrefix() {
    std::stringstream ss;
    ss << _nickname << "!" << _username << "@" << _ip;
    _prefix = ss.str();
}
