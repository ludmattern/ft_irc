/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 18:20:05 by lmattern          #+#    #+#             */
/*   Updated: 2024/08/20 18:21:08 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Client_HPP
#define Client_HPP

class Client {
    private:
        int memberVar;

    public:
        Client();
        Client(const Client& other);
        Client& operator=(const Client& other);
        ~Client();
};

#endif
