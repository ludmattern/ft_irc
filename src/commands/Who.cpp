/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 17:39:16 by lmattern          #+#    #+#             */
/*   Updated: 2024/10/05 17:48:15 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/Command.hpp"
#include "network/Server.hpp"
#include "network/Client.hpp"
#include "network/Channel.hpp"
#include "replies.hpp"

Who::Who() {}
Who::~Who() {}

void Who::execute(Client* client, const std::vector<std::string>& params)
{
    if (params.empty())
    {
        client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "WHO"));
        return;
    }

    std::string target = params[0];

    // Vérifier si le target est un canal
    if (target[0] == GLOBAL_CHANNEL || target[0] == LOCAL_CHANNEL)
    {
        Channel* channel = _server.getChannel(target);
        if (!channel)
        {
            // Le canal n'existe pas, envoyer RPL_ENDOFWHO avec une liste vide
            client->reply(RPL_ENDOFWHO(client->getNickname(), target));
            return;
        }

        // Parcourir les utilisateurs du canal
        const std::map<Client*, bool>& clients = channel->getClientsmap();
        for (std::map<Client*, bool>::const_iterator it = clients.begin(); it != clients.end(); ++it)
        {
            Client* targetClient = it->first;

            std::string status = "H";

            // Statut sur le canal (@ pour opérateur)
            if (channel->isOperator(targetClient))
                status += "@";

            // Construire et envoyer la réponse RPL_WHOREPLY
            client->reply(RPL_WHOREPLY(
                client->getNickname(),
                channel->getName(),
                targetClient->getUsername(),
                targetClient->getHostname(),
                _server.getName(),
                targetClient->getNickname(),
                status,
                "0", // Hopcount
                targetClient->getRealname()
            ));
        }

        // Envoyer RPL_ENDOFWHO pour indiquer la fin de la liste
        client->reply(RPL_ENDOFWHO(client->getNickname(), target));
    }
    else
    {
        // Si le target n'est pas un canal, vous pouvez gérer d'autres cas (par exemple, WHO sur un utilisateur ou un masque)
        client->reply(RPL_ENDOFWHO(client->getNickname(), target));
    }
}
