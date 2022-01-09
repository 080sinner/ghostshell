# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/10/14 17:50:34 by eozben            #+#    #+#              #
#    Updated: 2022/01/09 18:54:17 by fbindere         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
INC = -Iincludes
LIBS = -Llibft -lft -I$(HOME)/.brew/opt/readline/include -L$(HOME)/.brew/opt/readline/lib -lreadline
FLAGS = -o "minishell" -Wall -Wextra -Werror
LIBFT = ./libft/libft.a
EXECUTOR_PATH = ./executor/
PARSER_PATH = ./parser/
DATA_STRUCT_PATH = ./data_struct/
SYS_CALLS_PATH = ./sys_calls/
LEXER_PATH = ./lexer/
EXPANDER_PATH = ./expander/
SIGNAL_HANDLER_PATH = ./signals/
BUILTIN_PATH = ./built_in/
SRC_PATH = ./src/

EXECUTOR_FILES = execute_cmd.c executor.c execute_utils.c get_cmd_path.c
PARSER_FILES = cmd_arr.c parser.c input_control.c
DATA_STRUCT_FILES = nodes.c create_toks.c free_toks.c manipulate_toks.c 
SYS_CALLS_FILES = fd_calls.c process_calls.c
LEXER_FILES = check_functions.c lexer.c mark_expansions.c read_here_doc.c
EXPANDER_FILES = expander.c exp_wildcards.c exp_variables.c exp_here_doc.c \
				exp_var_content.c
SIGNAL_HANDLER_FILES = signal_handler.c
BUILTIN_FILES = builtin_handler.c cd.c echo.c env_utils.c env.c exit.c \
				export.c pwd.c unset.c
SRC_FILES = main.c header.c

SRCS = $(addprefix $(SRC_PATH), $(SRC_FILES))
SRCS += $(addprefix $(EXECUTOR_PATH), $(EXECUTOR_FILES))
SRCS += $(addprefix $(PARSER_PATH), $(PARSER_FILES))
SRCS += $(addprefix $(DATA_STRUCT_PATH), $(DATA_STRUCT_FILES))
SRCS += $(addprefix $(SYS_CALLS_PATH), $(SYS_CALLS_FILES))
SRCS += $(addprefix $(LEXER_PATH), $(LEXER_FILES))
SRCS += $(addprefix $(EXPANDER_PATH), $(EXPANDER_FILES))
SRCS += $(addprefix $(SIGNAL_HANDLER_PATH), $(SIGNAL_HANDLER_FILES))
SRCS += $(addprefix $(BUILTIN_PATH), $(BUILTIN_FILES))

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
