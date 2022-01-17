/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 20:32:45 by eozben            #+#    #+#             */
/*   Updated: 2022/01/17 16:24:03 by fbindere         ###   ########.fr       */
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
# include <sys/stat.h>

# define SIGINT 2
# define SIGHUP 1
# define SIGQUIT 3
# define SIGABRT 6
# define GENERAL_STATE -5
# define SQUOTED_STATE -2
# define DQUOTED_STATE -3
# define END -4
# define HERE_DOC -7
# define PIPEIN -3
# define PIPEOUT -2
# define ERROR -1
# define VAR_NAME 0
# define VAR_CONTENT 1
# define VAR_VALUE 0
# define NEW_NODE 1
# define TRUE 1
# define FALSE 0
# define EXIT 1
# define NO_EXIT 0
# define NORIGHTS 13
# define NOFILE 2
# define LOG 0
# define HEAD 0
# define NEW 1

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
	int				exit_status;
	int				process_lvl;
	pid_t			pid;
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

t_utils	g_utils;

/*built_in functions*/
void	ft_copy_env(char **environ, int skip_var, t_node **head);
int		echo(char **args);
int		unset(t_node *command, t_node **head);
int		export(t_node *command, t_node **head);
int		search_envvar(char *envvar, char **env);
int		print_pwd(void);
int		print_env(int declare_flag);
int		change_dir(char *path);
int		check_builtin(t_tok *command);
int		create_new_env(char **env, t_node **head);
int		exit_builtin(t_node **head, t_node *command, int process_lvl);
int		check_valid_var_name(char *varname);
int		execute_builtin(t_node *command, int process_lvl, t_node **head);
int		check_builtin(t_tok *command);
char	*ft_getenv(char *envvar, char **env);
void	ft_exit(int status, t_node **head);

/* data_structure functions*/
void	ft_dll_attach_tok(t_tok **head, t_tok *attachment);
void	ft_dll_insert_tok(t_tok **head, t_tok *attachment);
void	insert_sublist(t_tok *slot, t_tok *insert);
void	ft_dll_attach_node(t_node **head, t_node *attachment);
void	ft_dll_insert_tok(t_tok **head, t_tok *insert);
void	free_tok(t_tok **head, t_tok *tok);
int		free_toks(t_tok **head);
int		free_nodes(t_node **head);
t_node	*detach_node(t_node **head, t_node *node);
t_node	*ft_dll_append_node(t_node **head);
t_node	*ft_last_node(t_node *head);
t_tok	*ft_dll_append_tok(t_tok **head);
t_tok	*detach_tok(t_tok **head, t_tok *node);
t_tok	*ft_last_tok(t_tok *head);
t_tok	*create_new_tok(void);

/* executor functions*/
void	init_exec(t_exec *exec, int process_lvl, t_node **head);
void	executor(t_node *current, int process_lvl, t_node **head);
void	retrieve_here_doc(t_node *command, t_node **head);
void	execute_command(t_exec *exec, t_node **command, t_node **head);
void	execute_error(char *command, t_node **head);
int		is_pipeline(t_node *command);
int		create_array(t_node *command);
t_node	*skip_paren_content(t_node *current, int first_call);

/* expander functions*/
int		read_here_docs(t_node **head);
int		expand_here_doc(t_tok *here_doc);
int		expander(t_node *node);
int		expand_wildcards(t_tok **new, t_tok **tokhead);
int		general_variable(t_tok *new, char **varcontent, char *data);
int		get_cmd_path(t_node *command);
int		dquoted_var(char **varcontent, t_tok *new, int var_type);
t_tok	*expand_variable(char *data, char *varcontent, int var_type);

/* systemcalls functions*/
void	ft_close(int fd, char *function, t_node **head, int exit_flag);
void	ft_pipe(int *fds, char *function, t_node **head, int exit_flag);
void	ft_dup2(int fd1, int fd2, t_node **head, int exit_flag);
int		ft_fork(char *function, t_node **head, int exit_flag);
int		ft_dup(int fd, char *function, t_node **head, int exit_flag);
int		ft_open(char *file, int type);

/* lexer functions*/
int		lexer(t_node **head, char *input);
int		check_state(char **input, int *state, t_tok *new);
int		check_whitespace(char c);
int		is_control_op(t_token c);
int		check_expansion(char **input, int *state);
int		mark_variable(char **input, t_tok *new);
t_token	is_redir_op(char *s);
t_token	check_type(char *s);

/* parser functions*/
int		parse_command(t_node *current, t_node **head);
int		check_input(t_node **head);
int		set_redir(t_node *cmd, t_node **head, t_node *prev, t_node *next);
int		print_error_type(t_node *node, t_tok *token);
t_node	*search_lparen(t_node *current, int first_call);

/* singal handler functions*/
void	clear_signals(void);
void	signal_heredoc(int signum);
void	sig_ctrl(int signum);
int		signal_handler(void);

/* source functions*/
void	print_ghostshell(void);

#endif
