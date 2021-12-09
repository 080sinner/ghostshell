# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/10/14 17:50:34 by eozben            #+#    #+#              #
#    Updated: 2021/12/06 19:17:05 by fbindere         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
INC = -Iincludes
LIB = -Llibft -lft -lreadline
FLAGS = -o "minishell" -Wall -Wextra -Werror -g
SRC = srcs/dll_functions.c srcs/lexer_new.c srcs/main.c srcs/check_functions.c \
		srcs/dll_nodes.c srcs/dll_toks.c srcs/header.c srcs/variables.c \
		srcs/wildcards.c
all:$(NAME)

$(NAME): ./libft/libft.a $(SRC)
	@gcc $(FLAGS) $(SRC) $(INC) $(LIB)
	@echo "                                                         "
	@echo " \033[1;32m  ___|   _ \    \  |   _ \ _ _|  |      ____|  __ \   | "
	@echo "  |      |   |  |\/ |  |   |  |   |      __|    |   |  | "
	@echo "  |      |   |  |   |  ___/   |   |      |      |   | _| "
	@echo " \____| \___/  _|  _| _|    ___| _____| _____| ____/  _) \033[0m"
	@echo "                                                         "

./libft/libft.a:
	@echo ""
	@echo "libft:"
	@echo "\033[1;32m0%\033[0m   [                      ]"
	@sleep 0.5
	@echo "\033[1;32m33%\033[0m  [\033[1;32m ██████\033[0m               ]"
	@echo "\033[1;32m50%\033[0m  [\033[1;32m ██████████\033[0m           ]"
	@make -silent -C ./libft
	@echo "\033[1;32m83%\033[0m  [\033[1;32m ████████████████\033[0m     ]"
	@make clean -C ./libft
	@echo "\033[1;32m100%\033[0m [\033[1;32m ████████████████████ \033[0m]"

clean:
	@make clean -C ./libft
	@echo "\033[1;32m.o files removed!\033[0m"

fclean:
	@make fclean -C ./libft
	@rm -f $(NAME)
	@echo "\033[1;32mbinary files removed!\033[0m"

re: fclean all
