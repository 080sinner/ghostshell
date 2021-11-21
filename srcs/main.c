/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 20:47:32 by fbindere          #+#    #+#             */
/*   Updated: 2021/11/21 23:00:18 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// static void	print_list(t_node *head)
// {
// 	int i;

// 	i = 0;
// 	while (head != NULL)
// 	{
// 		while (head->args != NULL)
// 		{

// 			printf("toks: %d\n", i);
// 			i++;
// 			if(head->type == COMMAND)
// 				printf("%s\n", head->args->data);
// 			head->args = head->args->next;
// 		}
// 		printf("%c\n", head->type);
// 		head = head->next;
// 	}
// }

void	get_input(t_node **head, t_node **save)
{
	// char	*read;

	// while (1)
	// {
	// 	read = readline("minish $ ");
	// 	if (!ft_strcmp(read, "\0"))
	// 		add_history(read);
	// 	if (ft_strcmp(read, "exit"))
	// 	{
	// 		free(read);
	// 		break ;
	// 	}
		*save = read_toks(head, "\"echo |\" hello");
// 		free(read);
// 	}
}

int	main(void)
{
	t_node	*head;
	t_node	*save;

	head = NULL;
	save = NULL;
	get_input(&head, &save);
	// print_list(head);
	// free_list(save);
	// free_dll(&head);
	// save = NULL;
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
