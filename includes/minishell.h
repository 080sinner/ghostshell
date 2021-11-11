/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 20:32:45 by eozben            #+#    #+#             */
/*   Updated: 2021/11/11 16:39:09 by eozben           ###   ########.fr       */
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

#endif