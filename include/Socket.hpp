/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 18:16:08 by lmattern          #+#    #+#             */
/*   Updated: 2024/08/21 14:41:38 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <stdexcept>
#include <string>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

class Socket
{
public:
	Socket();
	~Socket();

	void bindSocket(int port);
	void listenSocket(int backlog = SOMAXCONN);
	int acceptClient(sockaddr_in *client_addr, socklen_t *addrlen);
	void setNonBlocking();

	int getFd() const;

private:
	int socket_fd;
};

#endif // SOCKET_HPP
