/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 20:32:45 by eozben            #+#    #+#             */
/*   Updated: 2021/12/21 17:40:37 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdio.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <errno.h>
# include <dirent.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <termios.h>

# define SIGINT 2
# define SIGHUP 1
# define SIGQUIT 3
# define SIGABRT 6

# define GENERAL_STATE -1
# define SQUOTED_STATE -2
# define DQUOTED_STATE -3
# define END -4
# define HERE_DOC -7
# define PIPEIN -3
# define PIPEOUT -2
# define ERROR -1
# define ENV_VAR_NAME 0
# define ENV_VAR_CONTENT 1
# define VAR_VALUE 0
# define NEW_NODE 1
# define NEW_TOK 2
# define CONTINUE 1
# define TMP 1
# define TRUE 1
# define FALSE 0
# define EVEN 0
# define ODD 1

typedef enum e_token
{
	COMMAND = 0,
	SQUOTE = '\'',
	DQUOTE = '\"',
	PIPE = '|',
	LPAREN = '(',
	RPAREN = ')',
	GREAT = '>',
	LESS = '<',
	AND = 152,
	LESSLESS = 240,
	GREATGREAT = 248,
	OR = 496,
	AMPERSAND = '&',
}			t_token;

typedef struct s_tok
{
	char			*data;
	int				state;
	int				type;
	struct s_tok	*next;
	struct s_tok	*previous;
}				t_tok;

typedef struct s_exec
{
	int				pipe[2];
	int				tmp_fd;
	pid_t			pid;
	int				exit_status;
}				t_exec;


typedef struct s_node
{
	t_tok			*args;
	t_tok			*here_doc;
	char			*cmdpath;
	char			**cmd_arr;
	int				in;
	int				out;
	t_token			type;
	struct s_node	*next;
	struct s_node	*previous;
}				t_node;

typedef struct s_utils
{
	char	**environment;
	pid_t	exit_status;
}				t_utils;

t_utils g_utils;

void	free_list(t_tok *head);
t_tok	*ft_last_tok(t_tok *head);
void	ft_dll_attach_tok(t_tok **head, t_tok *attachment);
void	ft_dll_insert_tok(t_tok **head, t_tok *attachment);
t_tok	*ft_dll_append_tok(t_tok **head, t_node **head_node);
t_node	*ft_dll_append_node(t_node **head);
t_tok	*detach_tok(t_tok **head, t_tok *node);
void	insert_sublist(t_tok *slot, t_tok *insert);
int		free_toks(t_tok **head);
int		free_nodes(t_node **head);
t_node	*detach_node(t_node **head, t_node *node);
t_token	check_type(char *s);
int		check_state(char **input, int *state, t_tok *new);
void	read_word(char **input, t_tok *token);
int		read_input(t_node **head, char *input);
int		check_whitespace(char c);
int		is_control_op(t_token c);
void	ft_dll_attach_node(t_node **head, t_node *attachment);
t_node	*ft_last_node(t_node *head);
int		expand_wildcards(t_tok **new, t_tok **tokhead, t_node **head);
t_tok	*expand_variable(char *data, t_node **head, char *varcontent, int tmp);
t_token	is_redir_op(char *s);
void	print_ghostshell(void);
int		here_doc(t_node *command, t_tok *here_doc, t_node **head);
int		lexer(t_node **head, char *input);
char	*ft_append(char *line, char c, t_node **head);
t_tok	*create_new_tok(t_tok **headtok, t_node **head);
void	read_here_docs(t_node **head);
void	expand_here_doc(t_tok *here_doc);
void	executor (t_node *current, t_node **head);
void	init_exec(t_exec *exec);
void	expander(t_node *node, t_node **head);
int		change_dir(char *path);
char	*ft_getenv(char *envvar, char **env);
void	get_cmd_path(t_node *command);
int		check_builtin(t_node *command, t_node **head);
void	parse_command(t_node *current, t_node **head);
void	ft_copy_env(char **environ, int skip_var);

#endif