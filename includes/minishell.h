/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 20:32:45 by eozben            #+#    #+#             */
/*   Updated: 2021/11/16 17:17:25 by eozben           ###   ########.fr       */
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

# define GREAT 401
# define LESS 402
# define GREATGREAT 403
# define LESSLESS 404
# define PIPE 405

typedef struct s_tok
{
	char			*data;
	int				type;
	struct s_tok	*next;
	struct s_tok	*previous;
}				t_tok;

typedef struct s_lexer
{
	t_tok	*tok_list;
	int		ntoks;
}				t_lexer;

typedef struct s_tree_node
{
	char				data;
	struct s_tree_node	*left;
	struct s_tree_node	*right;
}				t_tree;

typedef struct s_cmd_table
{
	char				**cmd;
	int					input;
	int					output;
	int					cmd_amount;
	struct s_cmd_table	*next;
	struct s_cmd_table	*prev;
}				t_cmd_table;

enum TOKEDEF
{
	blabla,
	blabla2,
	blblabla3,
};

t_tok	*ft_last_element(t_tok *head);
void	ft_dll_attach_tok(t_tok **head, t_tok *attachment);
void	ft_dll_insert_tok(t_tok **head, t_tok *attachment);
t_tok	*ft_dll_append_tok(t_tok **head);
t_tok	*detach_tok(t_tok **head, t_tok *node);

#endif