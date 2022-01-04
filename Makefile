# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eozben <eozben@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/10/14 17:50:34 by eozben            #+#    #+#              #
#    Updated: 2022/01/04 22:49:05 by eozben           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
INC = -Iincludes
LIBS = -Llibft -lft -I$(HOME)/.brew/opt/readline/include -L$(HOME)/.brew/opt/readline/lib -lreadline
FLAGS = -o "minishell" -Wall -Wextra -Werror
LIBFT = ./libft/libft.a
SRC_PATH = ./srcs/
SRC_FILES =	dll_functions.c lexer_new.c main.c check_functions.c dll_nodes.c \
			dll_toks.c header.c variables.c wildcards.c here_doc.c executor2.c \
			getpath.c built_ins.c error/ft_systemcalls.c input_check.c
SRCS = $(addprefix $(SRC_PATH), $(SRC_FILES))

all:$(NAME)

$(NAME): $(LIBFT) $(SRCS)
	@gcc $(FLAGS) $(SRCS) $(INC) $(LIBS)
	@echo "                                                         "
	@echo " \033[1;32m  ___|   _ \    \  |   _ \ _ _|  |      ____|  __ \   | "
	@echo "  |      |   |  |\/ |  |   |  |   |      __|    |   |  | "
	@echo "  |      |   |  |   |  ___/   |   |      |      |   | _| "
	@echo " \____| \___/  _|  _| _|    ___| _____| _____| ____/  _) \033[0m"
	@echo "                                                         "

$(LIBFT):
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
