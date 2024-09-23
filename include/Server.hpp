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

#define RPL_WELCOME "001"
#define RPL_YOURHOST "002"
#define RPL_CREATED "003"
#define RPL_MYINFO "004"
#define ERR_NEEDMOREPARAMS "461"
#define ERR_ALREADYREGISTRED "462"
#define ERR_PASSWDMISMATCH "464"
#define ERR_UNKNOWNCOMMAND "421"
#define ERR_NICKNAMEINUSE "433"
#define ERR_NONICKNAMEGIVEN "431"

class Server {
public:
    Server(int port, const std::string& password);
    ~Server();

    void run();

private:
    int _server_fd;
    int _port;
    std::string _password;
    std::vector<struct pollfd> _poll_fds;
    std::map<int, Client*> _clients;
    std::string _server_name;
    bool _is_running;

    void mod_event_to_pollout(int client_fd);
    void init_server_socket();
    void set_non_blocking(int fd);
    void accept_new_connection();
    void client_read(int client_fd);
    void client_write(int client_fd);
    void disconnect_client(int client_fd);

    typedef void (Server::*CommandHandler)(Client*, const std::string&);
    void initializeCommandHandlers();
    void processCommand(Client* client, const std::string& commandLine);
    std::map<std::string, CommandHandler> _commandHandlers;

    void handlePassCommand(Client* client, const std::string& params);
    void handleNickCommand(Client* client, const std::string& params);
    void handleUserCommand(Client* client, const std::string& params);
    void attemptClientRegistration(Client* client);

    void sendWelcomeMessages(Client* client);
    void displaytoServer(const std::string& message);
    void sendReply(Client* client, const std::string& code,
                   const std::string& params, const std::string& message);
    void sendError(Client* client, const std::string& code,
                   const std::string& command, const std::string& message);
    void sendToClient(Client* client, const std::string& message);
    bool isNicknameInUse(const std::string& nickname) const;
    bool hasClientToBeDisconnected(int client_fd);

    void cleanup();

};

#endif
