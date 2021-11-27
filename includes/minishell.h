/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 20:32:45 by eozben            #+#    #+#             */
/*   Updated: 2021/11/28 00:20:26 by eozben           ###   ########.fr       */
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

# define GENERAL_STATE 0
# define SQUOTED_STATE 1
# define DQUOTED_STATE 2
# define VAR_VALUE 0
# define NEW_NODE 1
# define NEW_TOK 2
# define CONTINUE 1
# define TMP 1
# define TRUE 1
# define FALSE 0

// typedef struct s_lexer
// {
// 	t_tok	*tok_list;
// 	int		ntoks;
// }				t_lexer;

// typedef struct s_tree_node
// {
// 	char				data;
// 	struct s_tree_node	*left;
// 	struct s_tree_node	*right;
// }				t_tree;

// typedef struct s_cmd_table
// {
// 	char				**cmd;
// 	int					input;
// 	int					output;
// 	int					cmd_amount;
// 	struct s_cmd_table	*next;
// 	struct s_cmd_table	*prev;
// }				t_cmd_table;

typedef enum e_token
{
	COMMAND = 0,
	SPACE = ' ',
	TAB = '\t',
	NEWLINE = '\n',
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
	struct s_tok	*next;
	struct s_tok	*previous;
}				t_tok;

typedef struct s_node
{
	t_tok			*args;
	t_token			type;
	struct s_node	*next;
	struct s_node	*previous;
}				t_node;

void	free_list(t_tok *head);
t_tok	*ft_last_tok(t_tok *head);
void	ft_dll_attach_tok(t_tok **head, t_tok *attachment);
void	ft_dll_insert_tok(t_tok **head, t_tok *attachment);
t_tok	*ft_dll_append_tok(t_tok **head);
t_node	*ft_dll_append_node(t_node **head);
t_tok	*detach_tok(t_tok **head, t_tok *node);
void	insert_sublist(t_tok *slot, t_tok *insert);
int		free_toks(t_tok **head);
int		free_nodes(t_node **head);
t_node	*detach_node(t_node **head, t_node *node);
t_token	check_type(char *s);
int		check_ctrlop_whitespace(int state, char **input);
int		check_state(char c, int *state, t_tok *token);
void	read_word(char **input, t_tok *token);
int		read_toks(t_node **head, char *input);
int		check_whitespace(char c);
int		is_control_op(t_token c);
void	ft_dll_attach_node(t_node **head, t_node *attachment);
t_node	*ft_last_node(t_node *head);
int		handle_wildcards(t_tok **new, t_tok **head);
int		expand_variable(int state, char **input, t_tok *new);
int		is_redir_op(t_node *node);
void	print_ghostshell(void);
int		here_doc(t_node *here_doc_node);
int		lexer(t_node **head, char *input);

#endif