/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 20:32:45 by eozben            #+#    #+#             */
/*   Updated: 2021/11/22 15:09:20 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdio.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>

# define GENERAL_STATE 0
# define SQUOTED_STATE 1
# define DQUOTED_STATE 2
# define ON 1
# define OFF 0
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
	VARIABLE = '$',
	AND = 152,
	LESSLESS = 240,
	GREATGREAT = 248,
	OR = 496,
	AMPERSAND = '&',
}			t_token;

typedef struct s_tok
{
	char			*data;
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
int		check_state(char c, int *state);
void	read_word(char **input, t_tok *token);
int		read_toks(t_node **head, char *input);
int		check_whitespace(char c);
int		is_control_op(t_token c);
void	ft_dll_attach_node(t_node **head, t_node *attachment);
t_node	*ft_last_node(t_node *head);

#endif