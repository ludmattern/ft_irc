/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 18:20:25 by lmattern          #+#    #+#             */
/*   Updated: 2024/08/20 18:21:27 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Channel_HPP
#define Channel_HPP

class Channel {
    private:
        int memberVar;

    public:
        Channel();
        Channel(const Channel& other);
        Channel& operator=(const Channel& other);
        ~Channel();
};

#endif
