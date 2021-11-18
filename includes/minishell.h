/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 20:32:45 by eozben            #+#    #+#             */
/*   Updated: 2021/11/18 23:09:02 by eozben           ###   ########.fr       */
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
	WORD = 0,
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
	t_token			type;
	struct s_tok	*next;
	struct s_tok	*previous;
}				t_tok;

t_tok	*ft_last_element(t_tok *head);
void	ft_dll_attach_tok(t_tok **head, t_tok *attachment);
void	ft_dll_insert_tok(t_tok **head, t_tok *attachment);
t_tok	*ft_dll_append_tok(t_tok **head);
t_tok	*detach_tok(t_tok **head, t_tok *node);
void	insert_sublist(t_tok *slot, t_tok *insert);
void	free_dll(t_tok **head);
t_token	check_type(char *s);
int		check_state(char c, int *state);
void read_word(char **input, t_tok *token);
char	*ft_append(char *line, char c);
void	read_toks(t_tok **head, char *input);
void	skip_whitespace(char **input);

#endif