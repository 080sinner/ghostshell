# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mac <mac@student.42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/10/14 17:50:34 by eozben            #+#    #+#              #
#    Updated: 2021/11/25 05:37:16 by mac              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
INC = -Iincludes
LIB = -Llibft -lft -lreadline
FLAGS = -o "minishell" -Wall -Wextra -Werror -g -fsanitize=address -fno-omit-frame-pointer  -static-libsan
SRC = srcs/dll_functions.c srcs/lexer_new.c srcs/main.c srcs/check_functions.c \
		srcs/dll_nodes.c srcs/dll_toks.c srcs/wildcards.c srcs/variables.c

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
	@echo "0%   █ █ █ █ █ █ █ █ █ █"
	@make -silent -C ./libft
	@echo "33%  \033[0;32m█ █ █\033[0m"
	@make clean -C ./libft
	@echo "50%  \033[0;32m█ █ █ █ █\033[0m"
	@sleep 1
	@echo "80%  \033[0;32m█ █ █ █ █ █ █ █\033[0m"
	@echo "100% \033[0;32m█ █ █ █ █ █ █ █ █ █\033[0m"

clean:
	@make clean -C ./libft
	@echo ".o files removed!"

fclean:
	@make fclean -C ./libft
	@rm -f $(NAME)
	@echo "all binary files removed!"

re: fclean all
