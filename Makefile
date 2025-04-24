# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abidaux <abidaux@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/24 14:51:49 by abidaux           #+#    #+#              #
#    Updated: 2025/04/24 15:50:33 by abidaux          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
CC = gcc
CFLAGS = -Wall -Werror -Wextra -g

SRC_DIR = src
OBJ_DIR = obj

# Couleurs
GREEN = \033[0;32m
YELLOW = \033[0;33m
BLUE = \033[0;34m
RESET = \033[0m

# Utilisation de wildcard pour trouver tous les fichiers .c
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(BLUE)Compiling: $<$(RESET)"

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -pthread
	@echo "$(GREEN)$(NAME) successfully compiled!$(RESET)"
	@echo "$(YELLOW)"
	@echo "      🍴                Je mange, donc je suis                    🍴     " 
	@echo "   o/   ____  _     _ _                       _                     \o  "
	@echo "  /|   |  _ \| |__ (_) | ___  ___  ___  _ __ | |__   ___ _ __ ___    |\ "
	@echo "  / \  | |_) | '_ \| | |/ _ \/ __|/ _ \| '_ \| '_ \ / _ \ '__/ __|  / \ "
	@echo "       |  __/| | | | | | (_) \__ \ (_) | |_) | | | |  __/ |  \__ \      "
	@echo "   🍴  |_|   |_| |_|_|_|\___/|___/\___/| .__/|_| |_|\___|_|  |___/   🍴 "
	@echo "   🍴                                  |_|                           🍴 "
	@echo "$(RESET)"

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(YELLOW)Object files removed$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(YELLOW)$(NAME) removed$(RESET)"

re: fclean all

.PHONY: all clean fclean re
