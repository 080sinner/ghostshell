/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 20:47:32 by fbindere          #+#    #+#             */
/*   Updated: 2021/11/22 13:39:08 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_list(t_node *head)
{
	int	i;

	i = 0;
	while (head != NULL)
	{
		while (head->args != NULL)
		{
			if (head->type == COMMAND)
				printf("%s", head->args->data);
			head->args = head->args->next;
		}
		printf("\n");
		head = head->next;
	}
}

void	get_input(t_node **head)
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
		read_toks(head, read);
		free(read);
	}
}

int	main(void)
{
	t_node	*head;

	head = NULL;
	get_input(&head);
	print_list(head);
	free_nodes(&head);
	// head = NULL;
	// system("leaks minishell");
	return (0);
}

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
