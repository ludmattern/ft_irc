/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 14:28:42 by lmattern          #+#    #+#             */
/*   Updated: 2024/08/21 17:43:27 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket()
{
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
        throw std::runtime_error("Failed to create socket");
}

Socket::~Socket()
{
    if (socket_fd != -1)
        close(socket_fd);
}

void Socket::bindSocket(int port)
{
    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    int reuse = 1;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
        throw std::runtime_error("Failed to set socket option SO_REUSEADDR");

    if (bind(socket_fd, (sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        throw std::runtime_error("Failed to bind socket");
}

void Socket::listenSocket(int backlog)
{
    if (listen(socket_fd, backlog) < 0)
        throw std::runtime_error("Failed to listen on socket");
}

int Socket::acceptClient(sockaddr_in *client_addr, socklen_t *addrlen)
{
    int client_fd = accept(socket_fd, (sockaddr *)client_addr, addrlen);
    if (client_fd < 0)
        throw std::runtime_error("Failed to accept client connection");
    return (client_fd);
}

void Socket::setNonBlocking()
{
    if (fcntl(socket_fd, F_SETFL, O_NONBLOCK) == -1)
        throw std::runtime_error("Failed to set socket to non-blocking mode");
}

int Socket::getFd() const
{
    return (socket_fd);
}
