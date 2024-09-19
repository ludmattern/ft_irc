// Server.hpp

#ifndef SERVER_HPP
#define SERVER_HPP

#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <netinet/in.h> // Pour sockaddr_in
#include "Client.hpp"

// Codes de réponse (RFC 1459)
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
    // Constructeur et destructeur
    Server(const std::string& password);
    ~Server();

    // Méthode pour démarrer le serveur
    void start(int port);

private:
    // Méthodes de gestion du serveur
    void setupServerSocket(int port);
    void acceptNewClient();
    void handleClientData(int client_fd);
    void disconnectClient(int client_fd);

    // Méthodes de traitement des clients
    void processClientBuffer(Client* client);
    void processClientCommand(Client* client, const std::string& command_line);

    // Gestionnaires de commandes
    typedef void (Server::*CommandHandler)(Client*, const std::string&);
    void initializeCommandHandlers();
    std::map<std::string, CommandHandler> _commandHandlers;

    // Gestion des commandes d'authentification
    void handlePassCommand(Client* client, const std::string& params);
    void handleNickCommand(Client* client, const std::string& params);
    void handleUserCommand(Client* client, const std::string& params);
    void attemptClientRegistration(Client* client);
    void sendWelcomeMessages(Client* client);

    // Fonctions utilitaires
    void sendReply(Client* client, const std::string& code,
                   const std::string& params, const std::string& message);
    void sendError(Client* client, const std::string& code,
                   const std::string& command, const std::string& message);
    void sendToClient(Client* client, const std::string& message);
    bool isNicknameInUse(const std::string& nickname) const;

    // Attributs du serveur
    int _server_fd;
    std::string _password;
    std::string _server_name;
    bool _is_running;

    // Map des clients connectés (clé : descripteur de socket)
    std::map<int, Client*> _clients;

    // Méthodes interdites par le projet
    Server();
    Server(const Server& other);
    Server& operator=(const Server& other);
};

#endif // SERVER_HPP
