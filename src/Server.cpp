// Server.cpp

#include "Server.hpp"
#include <iostream>
#include <cstring>      // Pour memset()
#include <fcntl.h>      // Pour fcntl()
#include <unistd.h>     // Pour close()
#include <arpa/inet.h>  // Pour inet_ntop()
#include <sys/socket.h> // Pour socket functions
#include <netdb.h>      // Pour getaddrinfo()
#include <poll.h>       // Pour poll()
#include <cstdio>       // Pour perror()

// Constructeur du serveur
Server::Server(const std::string& password)
    : _password(password), _server_name("irc.example.com"), _is_running(false) {
    initializeCommandHandlers();
}

// Destructeur du serveur
Server::~Server() {
    // Fermer le socket du serveur
    if (_server_fd > 0) {
        close(_server_fd);
    }
    // Déconnecter tous les clients
    for (std::map<int, Client*>::iterator it = _clients.begin();
         it != _clients.end(); ++it) {
        disconnectClient(it->first);
    }
}

// Démarrer le serveur
void Server::start(int port) {
    setupServerSocket(port);
    _is_running = true;

    // Vecteur pour poll()
    std::vector<struct pollfd> fds;

    // Ajouter le socket du serveur à la liste
    struct pollfd server_pollfd;
    server_pollfd.fd = _server_fd;
    server_pollfd.events = POLLIN;
    fds.push_back(server_pollfd);

    while (_is_running) {
        int poll_count = poll(&fds[0], fds.size(), -1);
        if (poll_count == -1) {
            perror("poll");
            break;
        }

        for (size_t i = 0; i < fds.size(); ++i) {
            if (fds[i].revents & POLLIN) {
                if (fds[i].fd == _server_fd) {
                    // Nouvelle connexion entrante
                    acceptNewClient();
                } else {
                    // Données reçues d'un client existant
                    int client_fd = fds[i].fd;
                    handleClientData(client_fd);
                }
            }
        }

        // Mise à jour de la liste des descripteurs (en cas de déconnexion)
        fds.clear();
        // Ajouter le socket du serveur
        server_pollfd.fd = _server_fd;
        server_pollfd.events = POLLIN;
        fds.push_back(server_pollfd);
        // Ajouter les sockets des clients
        for (std::map<int, Client*>::iterator it = _clients.begin();
             it != _clients.end(); ++it) {
            struct pollfd client_pollfd;
            client_pollfd.fd = it->first;
            client_pollfd.events = POLLIN;
            fds.push_back(client_pollfd);
        }
    }
}

// Configurer le socket du serveur
void Server::setupServerSocket(int port) {
    _server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_server_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Rendre le socket non bloquant
    if (fcntl(_server_fd, F_SETFL, O_NONBLOCK) == -1) {
        perror("fcntl");
        close(_server_fd);
        exit(EXIT_FAILURE);
    }

    // Réutilisation de l'adresse
    int opt = 1;
    if (setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        close(_server_fd);
        exit(EXIT_FAILURE);
    }

    // Liaison du socket à l'adresse et au port
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // Toutes les interfaces
    server_addr.sin_port = htons(port);

    if (bind(_server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(_server_fd);
        exit(EXIT_FAILURE);
    }

    // Écoute des connexions entrantes
    if (listen(_server_fd, SOMAXCONN) < 0) {
        perror("listen");
        close(_server_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Serveur démarré sur le port " << port << std::endl;
}

// Accepter une nouvelle connexion client
void Server::acceptNewClient() {
    struct sockaddr_in client_addr;
    socklen_t addrlen = sizeof(client_addr);

    int client_fd = accept(_server_fd, (struct sockaddr*)&client_addr, &addrlen);
    if (client_fd < 0) {
        perror("accept");
        return;
    }

    // Rendre le socket du client non bloquant
    if (fcntl(client_fd, F_SETFL, O_NONBLOCK) == -1) {
        perror("fcntl");
        close(client_fd);
        return;
    }

    // Obtenir l'adresse IP du client
    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(client_addr.sin_addr), ip_str, INET_ADDRSTRLEN);
    std::string client_ip(ip_str);

    // Créer un nouvel objet Client
    Client* new_client = new Client(client_fd, client_ip);

    // Ajouter le client à la map
    _clients[client_fd] = new_client;

    std::cout << "Nouvelle connexion : " << client_ip << std::endl;
}

// Gérer les données reçues d'un client
void Server::handleClientData(int client_fd) {
    char buffer[512];
    ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0) {
        // Le client a fermé la connexion ou une erreur est survenue
        disconnectClient(client_fd);
        return;
    }

    buffer[bytes_received] = '\0';
    std::string message(buffer);

    Client* client = _clients[client_fd];
    if (!client) {
        // Erreur : client non trouvé
        return;
    }

    // Ajouter les données au tampon du client
    client->appendToInputBuffer(message);

    // Traiter les commandes complètes
    processClientBuffer(client);
}

// Déconnecter un client
void Server::disconnectClient(int client_fd) {
    Client* client = _clients[client_fd];
    if (client) {
        std::cout << "Déconnexion du client : " << client->getIp() << std::endl;
        close(client_fd);
        delete client;
        _clients.erase(client_fd);
    }
}

// Traiter le tampon d'entrée du client
void Server::processClientBuffer(Client* client) {
    std::string& buffer = client->getInputBuffer();
    size_t pos;

    while ((pos = buffer.find("\r\n")) != std::string::npos) {
        std::string command_line = buffer.substr(0, pos);
        buffer.erase(0, pos + 2); // Supprimer la commande du tampon

        // Traiter la commande
        processClientCommand(client, command_line);
    }
}

// Traiter une commande du client
void Server::processClientCommand(Client* client, const std::string& command_line) {
    std::istringstream iss(command_line);
    std::string command;
    iss >> command;

    // Convertir la commande en majuscules
    std::transform(command.begin(), command.end(), command.begin(), ::toupper);

    // Récupérer les paramètres restants
    std::string params;
    std::getline(iss, params);
    if (!params.empty() && params[0] == ' ') {
        params.erase(0, 1); // Supprimer l'espace initial
    }

    // Rechercher le gestionnaire de commande
    std::map<std::string, CommandHandler>::iterator it = _commandHandlers.find(command);
    if (it != _commandHandlers.end()) {
        // Appeler le gestionnaire de commande
        CommandHandler handler = it->second;
        (this->*handler)(client, params);
    } else {
        // Commande inconnue
        sendError(client, ERR_UNKNOWNCOMMAND, command, "Unknown command");
    }
}

// Initialiser les gestionnaires de commandes
void Server::initializeCommandHandlers() {
    _commandHandlers["PASS"] = &Server::handlePassCommand;
    _commandHandlers["NICK"] = &Server::handleNickCommand;
    _commandHandlers["USER"] = &Server::handleUserCommand;
    // Ajouter d'autres commandes ici
}

// Gestionnaires de commandes

// Commande PASS
void Server::handlePassCommand(Client* client, const std::string& params) {
    if (client->isRegistered()) {
        sendError(client, ERR_ALREADYREGISTRED, "", "You may not reregister");
        return;
    }

    if (params.empty()) {
        sendError(client, ERR_NEEDMOREPARAMS, "PASS", "Not enough parameters");
        return;
    }

    client->setPassword(params);
}

// Commande NICK
void Server::handleNickCommand(Client* client, const std::string& params) {
    if (params.empty()) {
        sendError(client, ERR_NONICKNAMEGIVEN, "", "No nickname given");
        return;
    }

    std::string nickname = params;

    if (isNicknameInUse(nickname)) {
        sendError(client, ERR_NICKNAMEINUSE, nickname, "Nickname is already in use");
        return;
    }

    client->setNickname(nickname);

    // Vérifier si le client peut être enregistré
    attemptClientRegistration(client);
}

// Vérifier si un pseudonyme est déjà utilisé
bool Server::isNicknameInUse(const std::string& nickname) const {
    for (std::map<int, Client*>::const_iterator it = _clients.begin();
         it != _clients.end(); ++it) {
        if (it->second->getNickname() == nickname) {
            return true;
        }
    }
    return false;
}

// Commande USER
void Server::handleUserCommand(Client* client, const std::string& params) {
    if (client->isRegistered()) {
        sendError(client, ERR_ALREADYREGISTRED, "", "You may not reregister");
        return;
    }

    std::istringstream iss(params);
    std::string username, hostname, servername, realname;

    iss >> username >> hostname >> servername;
    std::getline(iss, realname);

    if (username.empty() || hostname.empty() || servername.empty() || realname.empty()) {
        sendError(client, ERR_NEEDMOREPARAMS, "USER", "Not enough parameters");
        return;
    }

    // Supprimer les deux-points initiaux du realname s'ils existent
    if (realname[0] == ':') {
        realname.erase(0, 1);
    }

    client->setUsername(username);
    client->setRealname(realname);

    // Vérifier si le client peut être enregistré
    attemptClientRegistration(client);
}

// Tenter l'enregistrement du client
void Server::attemptClientRegistration(Client* client) {
    if (client->getNickname().empty() || client->getUsername().empty()) {
        // Le client n'a pas encore fourni NICK ou USER
        return;
    }

    // Vérifier le mot de passe si nécessaire
    if (!_password.empty()) {
        if (client->getPassword() != _password) {
            sendError(client, ERR_PASSWDMISMATCH, "", "Password incorrect");
            disconnectClient(client->getFd());
            return;
        }
    }

    client->setRegistered(true);

    // Envoyer les messages de bienvenue
    sendWelcomeMessages(client);
}

// Envoyer les messages de bienvenue au client
void Server::sendWelcomeMessages(Client* client) {
    std::string prefix = client->getPrefix();
    sendReply(client, RPL_WELCOME, "", "Welcome to the IRC server " + prefix);
    sendReply(client, RPL_YOURHOST, "", "Your host is " + _server_name +
                                        ", running version 1.0");
    sendReply(client, RPL_CREATED, "", "This server was created today");
    sendReply(client, RPL_MYINFO, _server_name + " 1.0", "Available user modes: ...");
}

// Envoyer une réponse standard au client
void Server::sendReply(Client* client, const std::string& code,
                       const std::string& params, const std::string& message) {
    std::string nickname = client->getNickname();
    if (nickname.empty()) {
        nickname = "*";
    }
    std::string reply = ":" + _server_name + " " + code + " " + nickname;
    if (!params.empty()) {
        reply += " " + params;
    }
    if (!message.empty()) {
        reply += " :" + message;
    }
    reply += "\r\n";
    sendToClient(client, reply);
}

// Envoyer une erreur au client
void Server::sendError(Client* client, const std::string& code,
                       const std::string& command, const std::string& message) {
    std::string nickname = client->getNickname();
    if (nickname.empty()) {
        nickname = "*";
    }
    std::string error = ":" + _server_name + " " + code + " " + nickname;
    if (!command.empty()) {
        error += " " + command;
    }
    error += " :" + message + "\r\n";
    sendToClient(client, error);
}

// Envoyer un message brut au client
void Server::sendToClient(Client* client, const std::string& message) {
    send(client->getFd(), message.c_str(), message.length(), 0);
}

