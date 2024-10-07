# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/30 08:26:56 by lmattern          #+#    #+#              #
#    Updated: 2024/10/07 17:03:11 by lmattern         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY: all bonus clean fclean re

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -MMD -MP

SRCS_DIR = src
COMMAND_DIR = src/commands
INCLUDE_DIR = include
OBJS_DIR = build
BOT = bot

SRC_SRCS = $(wildcard $(SRCS_DIR)/*.cpp)
COMMAND_SRCS = $(wildcard $(COMMAND_DIR)/*.cpp)
SRCS = $(SRC_SRCS) $(COMMAND_SRCS)

OBJS = $(SRCS:%.cpp=$(OBJS_DIR)/%.o)

DEPS = $(OBJS:.o=.d)
NAME = ircserv

OBJS_DIRS = $(sort $(dir $(OBJS)))

all: $(NAME)

bonus : all $(BOT)
	make -C $(BOT)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -I $(INCLUDE_DIR) -o $@ $^

$(OBJS_DIRS):
	mkdir -p $@

$(OBJS_DIR)/%.o: %.cpp | $(OBJS_DIRS)
	$(CXX) $(CXXFLAGS) -I $(INCLUDE_DIR) -c $< -o $@

clean:
	rm -f $(OBJS) $(DEPS)
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME)
	rm -f $(BOT)/$(BOT)

re: fclean all

-include $(DEPS)
