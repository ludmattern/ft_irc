/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 13:47:41 by fprevot           #+#    #+#             */
/*   Updated: 2024/09/19 16:35:11 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client {
public:
	Client(int fd);
	~Client();

	int get_fd() const;
	void append_to_buffer(const char *data, size_t length);
	bool extract_command(std::string &command);

	void add_to_output_buffer(const std::string &data);
	const std::string &get_output_buffer() const;
	void erase_from_output_buffer(size_t length);

private:
	int _fd;
	std::string _input_buffer;
	std::string _output_buffer;
};

#endif
