/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 18:20:05 by lmattern          #+#    #+#             */
/*   Updated: 2024/08/21 17:06:33 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client
{
private:
	int _fd;
	std::string _ip_address;
	bool _authenticated;

public:
	Client(int fd, const std::string &ip_address);

	int getFd() const;
	const std::string &getIpAddress() const;
	bool isAuthenticated() const;
	void authenticate();
};

#endif
