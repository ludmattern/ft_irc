/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 13:47:41 by fprevot           #+#    #+#             */
/*   Updated: 2024/09/30 10:25:23 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <set>
#include <sstream>

#define MAX_BUFFER_SIZE 512
#define MAX_MESSAGE_LENGTH 510
#define CRLF "\r\n"
/**
 * Client class
 * This class manages a single client connection, including its state, data buffers,
 * and handling of IRC commands.
 */
class Client {
public:
    // Constructor and Destructor
    Client(int fd);
    ~Client();

    // === Getters ===
    int getFd() const;                              // Get the client's file descriptor
    const std::string& getIp() const;               // Get the client's IP address
    const std::string& getNickname() const;         // Get the client's nickname
    const std::string& getUsername() const;         // Get the client's username
    const std::string& getRealname() const;         // Get the client's real name
    const std::string& getPrefix() const;           // Get the client's IRC prefix
    const std::string& getPassword() const;         // Get the client's password
    bool isAuthenticated() const;                   // Check if the client is authenticated
    bool isRegistered() const;                      // Check if the client is registered
    bool shouldDisconnect() const;                  // Check if the client should be disconnected

    // === Setters ===
    void setIp(const std::string& ip);              // Set the client's IP address
    void setNickname(const std::string& nickname);  // Set the client's nickname
    void setUsername(const std::string& username);  // Set the client's username
    void setRealname(const std::string& realname);  // Set the client's real name
    void setPassword(const std::string& password);  // Set the client's password
    void setAuthenticated(bool authenticated);      // Set if the client is authenticated
    void setRegistered(bool registered);            // Set if the client is registered
    void markForDisconnection(bool to_disconnect);  // Mark the client to be disconnected

    // === Buffer handling ===
    void appendToInputBuffer(const char* data, size_t length);  // Append data to the input buffer
    bool extractCommand(std::string& command);                  // Extract a command from the input buffer
    void addToOutputBuffer(const std::string& data);            // Add data to the output buffer
    const std::string& getOutputBuffer() const;                 // Get the content of the output buffer
    void eraseFromOutputBuffer(size_t length);                  // Erase data from the output buffer
    std::string& getInputBuffer();                              // Get the input buffer content

    // === Channel management ===
    void joinChannel(const std::string& channel);               // Join a channel
    void leaveChannel(const std::string& channel);              // Leave a channel
    const std::set<std::string>& getChannels() const;           // Get the list of channels the client has joined

    // === Prefix generation ===
    void updatePrefix();                                        // Update the client's IRC prefix

private:
    int _fd;                             // File descriptor for the client
    std::string _ip;                     // Client's IP address
    std::string _nickname;               // Client's nickname
    std::string _username;               // Client's username
    std::string _realname;               // Client's real name
    std::string _password;               // Client's password
    bool _is_authenticated;              // Whether the client is authenticated
    bool _is_registered;                 // Whether the client is registered
    bool _to_disconnect;                 // Whether the client is marked for disconnection

    std::set<std::string> _channels;     // List of channels the client is part of
    std::string _prefix;                 // IRC prefix (nickname!username@ip)
    std::string _input_buffer;           // Buffer for incoming data from the client
    std::string _output_buffer;          // Buffer for outgoing data to the client
};

#endif
