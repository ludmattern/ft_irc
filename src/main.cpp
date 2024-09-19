/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 14:05:01 by lmattern          #+#    #+#             */
/*   Updated: 2024/09/19 14:05:03 by lmattern         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

// main.cpp

#include <iostream>
#include <cstdlib>    // Pour atoi()
#include "Server.hpp"

void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " <port> <password>" << std::endl;
    std::cout << "  <port>     : Port sur lequel le serveur écoute (1-65535)" << std::endl;
    std::cout << "  <password> : Mot de passe requis pour se connecter au serveur" << std::endl;
}

int main(int argc, char* argv[]) {
    // Vérifier le nombre d'arguments
    if (argc != 3) {
        printUsage(argv[0]);
        return EXIT_FAILURE;
    }

    // Récupérer le port
    int port = atoi(argv[1]);
    if (port <= 0 || port > 65535) {
        std::cerr << "Erreur: le port doit être un nombre entre 1 et 65535." << std::endl;
        return EXIT_FAILURE;
    }

    // Récupérer le mot de passe
    std::string password = argv[2];
    if (password.empty()) {
        std::cerr << "Erreur: le mot de passe ne peut pas être vide." << std::endl;
        return EXIT_FAILURE;
    }

    try {
        // Créer le serveur
        Server server(password);

        // Démarrer le serveur
        server.start(port);
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
