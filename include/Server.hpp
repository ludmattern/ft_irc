/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 18:21:43 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/28 14:32:15 by lmattern         ###   ########.fr       */
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
#define ERR_NOSUCHNICK       "401"
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

//Misc Constants
#define GLOBAL_CHANNEL '#'
#define LOCAL_CHANNEL '&'

// Colors
#define RESET       "\033[0m"
#define BLACK       "\033[30m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"

// Text styles
#define BOLD        "\033[1m"
#define UNDERLINE   "\033[4m"
#define BRED		"\033[1;31m"


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

	// Handlers for various IRC commands

	// Attempt to register a client (after PASS, NICK, USER)
	void registerClientIfReady(Client* client);

	// === Response and Error Handling ===
	void sendWelcomeMessages(Client* client); // Send welcome messages to the client
	void sendReply(Client* client, const std::string& code, const std::string& params, const std::string& message); // Send a standard reply
	void sendError(Client* client, const std::string& code, const std::string& command, const std::string& message); // Send an error message

	// Utility functions
	void logToServer(const std::string& message, const std::string& level); // Log a message to the server
	void sendRawMessageToClient(Client* client, const std::string& message); // Send a raw message to a client
	bool isNicknameTaken(const std::string& nickname) const; // Check if a nickname is already in use

	// Clean up resources
	void cleanupResources();

	Channel* getChannelByName(const std::string& channelName);
    Client* getClientByNickname(const std::string& nickname);

	// === Channel Handling ===
	Channel* getOrCreateChannel(const std::string& channelName);
	void addClientToChannel(Channel* channel, Client* client);
	void sendChannelInfoToClient(Channel* channel, Client* client);
	void broadcastToChannel(Channel* channel, const std::string& message, Client* sender);
	void removeChannel(const std::string& channelName);

private:
	// Server socket descriptor
	int _serverSocket;
	
	// Server parameters
	int _port;
	std::string _password;

	// Polling file descriptors, channels and client list
	std::vector<struct pollfd> _pollDescriptors;
	std::map<int, Client*> _clients;
	std::map<std::string, Channel*> _channels;

	// Server information
	std::string _serverName;
	bool _isRunning;



	void handlePollEvent(struct pollfd& pollDescriptor);
	bool isServerSocket(const struct pollfd& pollDescriptor) const;
	bool isClientSocket(const struct pollfd& pollDescriptor) const;
	void checkClientEvents(struct pollfd& pollDescriptor);
	void initializeServerSocket();          // Initialize the server socket
	void setSocketNonBlocking(int fd);      // Set a socket to non-blocking mode
	void handleNewConnection();       // Accept a new client connection
	void handleClientDisconnection(struct pollfd& pollDescriptor);
	void readFromClient(int clientFd);    // Read data from a client
	void writeToClient(int clientFd);   // Write data to a client
	void closeClientConnection(int clientFd); // Disconnect a client
	void setPollToWrite(int clientFd); // Modify poll event to POLLOUT for a client
	bool shouldClientDisconnect(int clientFd); // Check if a client should be disconnected

		// === Network event handling ===

	// Signal handling (e.g., server shutdown via Ctrl+C)
	static void handleSignal(int signal);
	void setRunningState(bool isRunning);

	// Argument parsing for server startup
	void parseArguments(int argc, char **argv);

	// === IRC Command Handling ===
	typedef void (Server::*CommandHandler)(Client*, const std::vector<std::string>&);

	std::map<std::string, CommandHandler> _commandHandlers; // Map of IRC commands to their handlers

	// Initialize command handlers
	void setupCommandHandlers();

	// Process a command sent by a client
	void processClientCommand(Client* client, const std::string& commandLine);
};

#endif
