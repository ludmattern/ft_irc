/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 02:52:24 by fprevot           #+#    #+#             */
/*   Updated: 2024/10/06 02:52:35 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_H
#define BOT_H

#include <string>
#include <vector>

std::vector<std::string> loadJokes(const std::string& filename);
void run(int sockfd, const std::string& channel, const std::vector<std::string>& jokes);
void handle_sigint(int signum);
void sendMessage(int sockfd, const std::string& message);

#endif