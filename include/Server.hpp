/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 18:21:43 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/24 15:23:34 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <map>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "Client.hpp"
#include "Channel.hpp"

// IRC Standard Replies (RPL)
#define RPL_WELCOME        "001"
#define RPL_YOURHOST       "002"
#define RPL_CREATED        "003"
#define RPL_MYINFO         "004"
#define RPL_TOPIC          "332"
#define RPL_NAMREPLY       "353"
#define RPL_ENDOFNAMES     "366"
#define RPL_NOTOPIC        "331"
#define RPL_TOPIC          "332"

// Other Numeric Replies
#define ERR_NOSUCHCHANNEL    "403"
#define ERR_CANNOTSENDTOCHAN "404"
#define ERR_TOOMANYCHANNELS  "405"
#define ERR_WASNOSUCHNICK    "406"
#define ERR_TOOMANYTARGETS   "407"
#define ERR_NOORIGIN         "409"
#define ERR_NORECIPIENT      "411"
#define ERR_NOTEXTTOSEND     "412"
#define ERR_UNKNOWNCOMMAND   "421"
#define ERR_NONICKNAMEGIVEN  "431"
#define ERR_ERRONEUSNICKNAME "432"
#define ERR_NICKNAMEINUSE    "433"
#define ERR_USERNOTINCHANNEL "441"
#define ERR_NOTONCHANNEL     "442"
#define ERR_USERONCHANNEL    "443"
#define ERR_NOTREGISTERED    "451"

// IRC Standard Errors
#define ERR_NEEDMOREPARAMS "461"
#define ERR_ALREADYREGISTRED "462"
#define ERR_PASSWDMISMATCH "464"
#define ERR_UNKNOWNCOMMAND "421"
#define ERR_NICKNAMEINUSE  "433"
#define ERR_NONICKNAMEGIVEN "431"

// Server Params
#define MIN_PORT 1
#define MAX_PORT 65535
#define RESERVED_PORT 1024
/**
 * Server class
 * This class handles the operation of the IRC server, including client connections,
 * command processing, and event-driven networking using `poll()`.
 */
class Server {
public:
	// Constructor and Destructor
	Server(int argc, char **argv);
	~Server();

	// Main method to run the server
	void run();

private:
	// Server socket descriptor
	int _server_fd;
	
	// Server parameters
	int _port;
	std::string _password;

	// Polling file descriptors, channels and client list
	std::vector<struct pollfd> _poll_fds;
	std::map<int, Client*> _clients;
	std::map<std::string, Channel*> _channels;

	// Server information
	std::string _server_name;
	bool _is_running;

	// === Network event handling ===
	void initializeServerSocket();          // Initialize the server socket
	void setSocketNonBlocking(int fd);      // Set a socket to non-blocking mode
	void handleNewConnection();       // Accept a new client connection
	void readFromClient(int client_fd);    // Read data from a client
	void writeToClient(int client_fd);   // Write data to a client
	void closeClientConnection(int client_fd); // Disconnect a client
	void setPollToWrite(int client_fd); // Modify poll event to POLLOUT for a client
	bool shouldClientDisconnect(int client_fd); // Check if a client should be disconnected

	// Signal handling (e.g., server shutdown via Ctrl+C)
	static void handleSignal(int signal);
	void setRunningState(bool is_running);

	// Argument parsing for server startup
	void parseArguments(int argc, char **argv);

	// === IRC Command Handling ===
	typedef void (Server::*CommandHandler)(Client*, const std::string&);

	std::map<std::string, CommandHandler> _commandHandlers; // Map of IRC commands to their handlers

	// Initialize command handlers
	void setupCommandHandlers();

	// Process a command sent by a client
	void processClientCommand(Client* client, const std::string& commandLine);

	// Handlers for various IRC commands
	void handlePassCommand(Client* client, const std::string& params);
	void handleNickCommand(Client* client, const std::string& params);
	void handleUserCommand(Client* client, const std::string& params);
	void handleJoinCommand(Client* client, const std::string& params);
	bool validateJoinCommand(Client* client, const std::string& params);
    void handlePrivmsgCommand(Client* client, const std::string& params);
	void handlePartCommand(Client* client, const std::string& params);
    void handleNoticeCommand(Client* client, const std::string& params);
    void handleQuitCommand(Client* client, const std::string& params);
    void handleTopicCommand(Client* client, const std::string& params);
    void handlePingCommand(Client* client, const std::string& params);
    void handlePongCommand(Client* client, const std::string& params);
    void handleKickCommand(Client* client, const std::string& params);
    void handleInviteCommand(Client* client, const std::string& params);

	// Attempt to register a client (after PASS, NICK, USER)
	void registerClientIfReady(Client* client);

	// === Response and Error Handling ===
	void sendWelcomeMessages(Client* client); // Send welcome messages to the client
	void sendReply(Client* client, const std::string& code, const std::string& params, const std::string& message); // Send a standard reply
	void sendError(Client* client, const std::string& code, const std::string& command, const std::string& message); // Send an error message

	// Utility functions
	void logToServer(const std::string& message); // Display a message to the server console
	void sendRawMessageToClient(Client* client, const std::string& message); // Send a raw message to a client
	bool isNicknameTaken(const std::string& nickname) const; // Check if a nickname is already in use

	// Clean up resources
	void cleanupResources();

	// === Channel Handling ===
    Channel* getOrCreateChannel(const std::string& channelName);
    void addClientToChannel(Channel* channel, Client* client);
    void sendChannelInfoToClient(Channel* channel, Client* client);
	void broadcastToChannel(Channel* channel, const std::string& message, Client* sender);
};

#endif
