# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/07 17:03:26 by lmattern          #+#    #+#              #
#    Updated: 2024/10/07 17:03:27 by lmattern         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY: all clean fclean re

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -MMD -MP

OBJS_DIR = build

SRCS = bot.cpp run.cpp

OBJS = $(SRCS:%.cpp=$(OBJS_DIR)/%.o)

DEPS = $(OBJS:.o=.d)

NAME = bot

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -I . -o $@ $^

$(OBJS_DIR):
	mkdir -p $@

$(OBJS_DIR)/%.o: %.cpp | $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) -I . -c $< -o $@

clean:
	rm -f $(OBJS) $(DEPS)
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(DEPS)
