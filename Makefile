.PHONY: all clean fclean re

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -MMD -MP

SRCS_DIR = src
INCLUDE_DIR = include
OBJS_DIR = build
COMMAND_DIR = src/Command

COMMAND_FILES = Pass Nick User Join PrivMsg Quit Notice Part Topic Ping Pong Kick Invite
SRC_FILES = main ServerUtils ServerSendings ServerClientHandling Client Channel Parser

# Ajout des fichiers dans src et dans src/Command
SRCS = $(addprefix $(SRCS_DIR)/,$(addsuffix .cpp,$(SRC_FILES))) \
       $(addprefix $(COMMAND_DIR)/,$(addsuffix .cpp,$(COMMAND_FILES)))

# Création des fichiers objets pour tous les fichiers sources
OBJS = $(patsubst $(SRCS_DIR)/%.cpp,$(OBJS_DIR)/%.o,$(SRCS))

DEPS = $(OBJS:.o=.d)
NAME = ircserv

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -I $(INCLUDE_DIR) -o $@ $^

# Compilation des objets pour les fichiers dans src/
$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp | $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) -I $(INCLUDE_DIR) -c $< -o $@

# Compilation des objets pour les fichiers dans src/Command/
$(OBJS_DIR)/%.o: $(COMMAND_DIR)/%.cpp | $(OBJS_DIR)
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
