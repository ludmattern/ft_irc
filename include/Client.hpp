// Client.hpp

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <set>

class Client {
public:
    Client(int fd, const std::string& ip);
    ~Client();

    // Accesseurs
    int getFd() const;
    const std::string& getIp() const;
    const std::string& getNickname() const;
    const std::string& getUsername() const;
    const std::string& getRealname() const;
    const std::string& getPrefix() const;
    const std::string& getPassword() const;
    bool isAuthenticated() const;
    bool isRegistered() const;

    // Modificateurs
    void setNickname(const std::string& nickname);
    void setUsername(const std::string& username);
    void setRealname(const std::string& realname);
    void setPassword(const std::string& password);
    void setAuthenticated(bool authenticated);
    void setRegistered(bool registered);

    // Tampon d'entrée
    void appendToInputBuffer(const std::string& data);
    std::string& getInputBuffer();

    // Gestion des canaux
    void joinChannel(const std::string& channel);
    void leaveChannel(const std::string& channel);
    const std::set<std::string>& getChannels() const;

    // Génération du préfixe
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
    std::string _input_buffer;
    std::set<std::string> _channels;
    std::string _prefix;
};

#endif // CLIENT_HPP
