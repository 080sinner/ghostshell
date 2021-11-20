/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mac <mac@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 20:47:32 by fbindere          #+#    #+#             */
/*   Updated: 2021/11/20 05:10:56 by mac              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	print_list(t_tok *head)
{
	while (head != NULL)
	{
		if (head->type == WORD)
			printf("%s\n", head->data);
		head = head->next;
	}
}

void get_input(t_tok **head, t_tok **save)
{
	char	*read;

	while (1)
	{
		read = readline("minish $ ");
		if (!ft_strcmp(read, "\0"))
			add_history(read);
		if (ft_strcmp(read, "exit"))
		{
			free(read);
			break ;
		}
		*save = read_toks(head, read);
		free(read);
	}
}

int	main(void)
{
	t_tok	*head;
	t_tok	*save;

	head = NULL;
	save = NULL;
	get_input(&head, &save);
	print_list(head);
	free_list(save);
	free_dll(&head);
	save = NULL;
	head = NULL;
	system("leaks minishell");
	return (0);
}

// typedef struct s_treenode{
// 	t_token tok;
// 	struct s_treenode *left;
// 	struct s_treenode *right;
// }				t_treenode;


// t_treenode	*create_tree_node(t_token token)
// {
// 	t_treenode	*new;

// 	new = malloc(sizeof(t_treenode));
// 	if (new != NULL)
// 	{
// 		new->left = NULL;
// 		new->right = NULL;
// 		new->tok = token;
// 	}
// 	return (new);
// }
