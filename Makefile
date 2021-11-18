# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/10/14 17:50:34 by eozben            #+#    #+#              #
#    Updated: 2021/11/18 21:20:39 by fbindere         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
INC = -Iincludes
LIB = -Llibft -lft -lreadline
FLAGS = -o "minishell" -Wall -Wextra -Werror
SRC = srcs/dll_functions.c srcs/dll_functions2.c srcs/lexer.c \
	srcs/main.c
all:$(NAME)

$(NAME): ./libft/libft.a
	@gcc $(FLAGS) $(SRC) $(INC) $(LIB)
	@echo "program compiled!"

./libft/libft.a:
	@make -silent -C ./libft
	@echo "library compiled!"

clean:
	@make clean -C ./libft
	@echo ".o files removed!"

fclean:
	@make fclean -C ./libft
	@echo ".a file and .o files removed!"

re: fclean all
