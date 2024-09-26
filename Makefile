.PHONY: all clean fclean re

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -MMD -MP

SRCS_DIR = src
COMMAND_DIR = src/Command
INCLUDE_DIR = include
OBJS_DIR = build

# Récupérer tous les fichiers .cpp dans src et src/Command
SRC_SRCS = $(wildcard $(SRCS_DIR)/*.cpp)
COMMAND_SRCS = $(wildcard $(COMMAND_DIR)/*.cpp)
SRCS = $(SRC_SRCS) $(COMMAND_SRCS)

# Création des fichiers objets pour tous les fichiers sources
OBJS = $(SRCS:%.cpp=$(OBJS_DIR)/%.o)

DEPS = $(OBJS:.o=.d)
NAME = ircserv

# Ajouter les répertoires source à VPATH
VPATH = $(SRCS_DIR):$(COMMAND_DIR)

# Gérer les répertoires des objets
OBJS_DIRS = $(sort $(dir $(OBJS)))

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -I $(INCLUDE_DIR) -o $@ $^

# Assurer que les répertoires des objets existent
$(OBJS_DIRS):
	mkdir -p $@

# Règle de compilation pour tous les fichiers objets
$(OBJS_DIR)/%.o: %.cpp | $(OBJS_DIRS)
	$(CXX) $(CXXFLAGS) -I $(INCLUDE_DIR) -c $< -o $@

clean:
	rm -f $(OBJS) $(DEPS)
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(DEPS)
