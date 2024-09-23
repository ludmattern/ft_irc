#include "Client.hpp"

Client::Client(int fd)
    : _fd(fd), _is_authenticated(false), _is_registered(false), _to_disconnect(false) {}

Client::~Client() {}

// Getters
int Client::getFd() const { return _fd; }
const std::string& Client::getIp() const { return _ip; }
const std::string& Client::getNickname() const { return _nickname; }
const std::string& Client::getUsername() const { return _username; }
const std::string& Client::getRealname() const { return _realname; }
const std::string& Client::getPassword() const { return _password; }
const std::string& Client::getPrefix() const { return _prefix; }
bool Client::isAuthenticated() const { return _is_authenticated; }
bool Client::isRegistered() const { return _is_registered; }
bool Client::hasToDisconnect() const { return _to_disconnect; }


// Setters
void Client::setIp(const std::string& ip) {
    _ip = ip;
    updatePrefix();
}

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

void Client::setToDisconnect(bool to_disconnect) {
    _to_disconnect = to_disconnect;
}

// Buffer handling
void Client::appendToInputBuffer(const char* data, size_t length) {
    _input_buffer.append(data, length);
}

bool Client::extractCommand(std::string& command) {
    size_t pos = _input_buffer.find("\r\n");
    if (pos != std::string::npos) {
        command = _input_buffer.substr(0, pos);
        _input_buffer.erase(0, pos + 2);
        return true;
    }
    return false;
}

void Client::addToOutputBuffer(const std::string& data) {
    _output_buffer.append(data);
}

const std::string& Client::getOutputBuffer() const {
    return _output_buffer;
}

void Client::eraseFromOutputBuffer(size_t length) {
    _output_buffer.erase(0, length);
}

std::string& Client::getInputBuffer() {
    return _input_buffer;
}

// Channel management
void Client::joinChannel(const std::string& channel) {
    _channels.insert(channel);
}

void Client::leaveChannel(const std::string& channel) {
    _channels.erase(channel);
}

const std::set<std::string>& Client::getChannels() const {
    return _channels;
}

// Prefix generation
void Client::updatePrefix() {
    std::stringstream ss;
    ss << _nickname << "!" << _username << "@" << _ip;
    _prefix = ss.str();
}
