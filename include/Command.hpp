#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <vector>
#include <string>

class Server;
class Client;

class Command {
public:
    Command() {}
    virtual ~Command() {}
    virtual void execute(Server& server, Client& client, const std::vector<std::string>& params) = 0;
};


class Pass : public Command {
public:
    Pass() {}
    virtual ~Pass() {}
    virtual void execute(Server& server, Client& client, const std::vector<std::string>& params);
};

class Nick : public Command {
public:
    Nick() {}
    virtual ~Nick() {}
    virtual void execute(Server& server, Client& client, const std::vector<std::string>& params);
};

class User : public Command {
public:
    User() {}
    virtual ~User() {}
    virtual void execute(Server& server, Client& client, const std::vector<std::string>& params);
};

class Join : public Command {
public:
    Join() {}
    virtual ~Join() {}
    virtual void execute(Server& server, Client& client, const std::vector<std::string>& params);
};

class PrivMsg : public Command {
public:
    PrivMsg() {}
    virtual ~PrivMsg() {}
    virtual void execute(Server& server, Client& client, const std::vector<std::string>& params);
};

class Quit : public Command {
public:
    Quit() {}
    virtual ~Quit() {}
    virtual void execute(Server& server, Client& client, const std::vector<std::string>& params);
};

class Notice : public Command {
public:
    Notice() {}
    virtual ~Notice() {}
    virtual void execute(Server& server, Client& client, const std::vector<std::string>& params);
};

class Part : public Command {
public:
    Part() {}
    virtual ~Part() {}
    virtual void execute(Server& server, Client& client, const std::vector<std::string>& params);
};

class Topic : public Command {
public:
    Topic() {}
    virtual ~Topic() {}
    virtual void execute(Server& server, Client& client, const std::vector<std::string>& params);
};

class Ping : public Command {
public:
    Ping() {}
    virtual ~Ping() {}
    virtual void execute(Server& server, Client& client, const std::vector<std::string>& params);
};

class Pong : public Command {
public:
    Pong() {}
    virtual ~Pong() {}
    virtual void execute(Server& server, Client& client, const std::vector<std::string>& params);
};

class Kick : public Command {
public:
    Kick() {}
    virtual ~Kick() {}
    virtual void execute(Server& server, Client& client, const std::vector<std::string>& params);
};

class Invite : public Command {
public:
    Invite() {}
    virtual ~Invite() {}
    virtual void execute(Server& server, Client& client, const std::vector<std::string>& params);
};

// Vous pouvez ajouter d'autres commandes ici
class CommandFactory {
public:
    static Command* createCommand(const std::string& commandName);
};

#endif // COMMAND_HPP
