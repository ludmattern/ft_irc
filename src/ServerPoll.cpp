/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerPoll.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 17:31:01 by lmattern          #+#    #+#             */
/*   Updated: 2024/08/21 17:43:39 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <stdexcept>
#include <poll.h>

void Server::checkPoll()
{
	int poll_count = poll(_SocketPollList.data(), _SocketPollList.size(), -1);
	if (poll_count == -1)
		throw std::runtime_error("poll() failed");
}

void Server::processPollEvent(pollfd &pfd)
{
	if (!(pfd.revents & POLLIN))
		return ;

	if (pfd.fd == _Socket.getFd())
		handleNewClient();
	else
		handleClientData(pfd.fd);
}
