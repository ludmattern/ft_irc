/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 13:47:36 by fprevot           #+#    #+#             */
/*   Updated: 2024/09/19 14:39:15 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"


int Client::get_fd() const
{
	return (_fd);
}

void Client::append_to_buffer(const char *data, size_t length)
{
	_input_buffer.append(data, length);
}

bool Client::extract_command(std::string &command)
{
	size_t pos = _input_buffer.find("\r\n");
	if (pos != std::string::npos)
	{
		command = _input_buffer.substr(0, pos);
		_input_buffer.erase(0, pos + 2);
		return (true);
	}
	return (false);
}

void Client::add_to_output_buffer(const std::string &data)
{
	_output_buffer.append(data);
}

const std::string &Client::get_output_buffer() const
{
	return (_output_buffer);
}

void Client::erase_from_output_buffer(size_t length)
{
	_output_buffer.erase(0, length);
}
