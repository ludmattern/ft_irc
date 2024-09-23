/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 13:47:41 by fprevot           #+#    #+#             */
/*   Updated: 2024/09/23 14:18:24 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <set>
#include <sstream>

class Client {
public:
    Client(int fd);
    ~Client();

    // Getters
    int getFd() const;
    const std::string& getIp() const;
    const std::string& getNickname() const;
    const std::string& getUsername() const;
    const std::string& getRealname() const;
    const std::string& getPrefix() const;
    const std::string& getPassword() const;
    bool isAuthenticated() const;
    bool isRegistered() const;
    bool hasToDisconnect() const;

    // Setters
    void setIp(const std::string& ip);
    void setNickname(const std::string& nickname);
    void setUsername(const std::string& username);
    void setRealname(const std::string& realname);
    void setPassword(const std::string& password);
    void setAuthenticated(bool authenticated);
    void setRegistered(bool registered);
    void setToDisconnect(bool to_disconnect);

    // Buffer handling
    void appendToInputBuffer(const char* data, size_t length);
    bool extractCommand(std::string& command);
    void addToOutputBuffer(const std::string& data);
    const std::string& getOutputBuffer() const;
    void eraseFromOutputBuffer(size_t length);
    std::string& getInputBuffer();

    // Channel management
    void joinChannel(const std::string& channel);
    void leaveChannel(const std::string& channel);
    const std::set<std::string>& getChannels() const;

    // Prefix generation
    void updatePrefix();

private:
    int _fd;
    std::string _ip;
    std::string _nickname;
    std::string _username;
    std::string _realname;
    std::string _password;
    bool _is_authenticated;
    bool _is_registered;
    bool _to_disconnect;
    std::set<std::string> _channels;
    std::string _prefix;
    std::string _input_buffer;
    std::string _output_buffer;
};

#endif
