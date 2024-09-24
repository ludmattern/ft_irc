# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/03 18:12:42 by lmattern          #+#    #+#              #
#    Updated: 2024/09/24 10:45:25 by lmattern         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY: all clean fclean re

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -MMD -MP

SRCS_DIR = src
INCLUDE_DIR = include
OBJS_DIR = build

SRC_FILES = main ServerUtils ServerSendings ServerCommands ServerClientHandling Client Channel
SRCS = $(addprefix $(SRCS_DIR)/,$(addsuffix .cpp,$(SRC_FILES)))
OBJS = $(patsubst $(SRCS_DIR)/%.cpp,$(OBJS_DIR)/%.o,$(SRCS))
DEPS = $(OBJS:.o=.d)
NAME = ircserv

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -I $(INCLUDE_DIR) -o $@ $^

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp | $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) -I $(INCLUDE_DIR) -c $< -o $@

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

clean:
	rm -f $(OBJS) $(DEPS)
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(DEPS)