/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 10:43:51 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/24 11:28:46 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <set>
#include "Client.hpp"

class Client;

class Channel {
public:
    // Constructeur
    Channel(const std::string& name);
    ~Channel();

    // Getters
    const std::string& getName() const;
    const std::string& getTopic() const;

    // Setters
    void setTopic(const std::string& topic);

    // Gestion des clients
    void addClient(Client* client);
    void removeClient(Client* client);
    bool hasClient(Client* client) const;
    const std::set<Client*>& getClients() const;

    // Gestion des modes
    void addMode(char mode);
    void removeMode(char mode);
    bool hasMode(char mode) const;

    // Envoi de messages
    void broadcastMessage(const std::string& message, Client* sender = NULL);

private:
    std::string _name;
    std::string _topic;
    std::set<Client*> _clients;
    std::set<char> _modes;
};

#endif
