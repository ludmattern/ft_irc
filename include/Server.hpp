/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 18:16:08 by lmattern          #+#    #+#             */
/*   Updated: 2024/08/20 18:18:39 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Server_HPP
#define Server_HPP

class Server {
    private:
        int memberVar;

    public:
        Server();
        Server(const Server& other);
        Server& operator=(const Server& other);
        ~Server();
};

#endif
