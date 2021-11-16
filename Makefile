# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eozben <eozben@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/10/14 17:50:34 by eozben            #+#    #+#              #
#    Updated: 2021/11/16 19:31:55 by eozben           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
INC = -Iincludes
LIB = -Llibft -lft 
FLAGS = -o "minishell" -Wall -Wextra -Werror

all:$(NAME)

$(NAME): ./libft/libft.a
	@gcc $(FLAGS) srcs/minishell.c srcs/utils.c dll/ft_dll_create.c dll/ft_dll_insert_tok.c dll/dll_listception.c dll/ft_dll_last.c dll/ft_dll_attach_tok.c $(INC) $(LIB)
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
