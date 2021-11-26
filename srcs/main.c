/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 20:47:32 by fbindere          #+#    #+#             */
/*   Updated: 2021/11/26 19:54:54 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_list(t_node *head)
{
	int		i;
	t_node	*tmpnode;
	t_tok	*tmptok;

	i = 0;
	tmpnode = head;
	while (tmpnode != NULL)
	{
		tmptok = tmpnode->args;
		while (tmptok != NULL)
		{
			if (tmpnode->type == COMMAND)
				printf("%s", tmptok->data);
			tmptok = tmptok->next;
		}
		if (tmpnode->type != COMMAND)
			printf("%c", tmpnode->type);
		printf("\n");
		tmpnode = tmpnode->next;
	}
}

static int	check_empty_input(char *input)
{
	int	i;

	i = 0;
	while (check_whitespace(input[i]))
		i++;
	if (input[i] == '\0')
		return (1);
	else
		return (0);
}

void	get_input(t_node **head)
{
	char	*read;

	print_ghostshell();
	while (1)
	{
		read = readline("\e[1m	\033[1;34m༼ つ ❍_❍ ༽つ\033[0m\e[0m	");
		if (read != NULL && !ft_strcmp(read, ""))
		{
			add_history(read);
			if (ft_strcmp(read, "exit"))
			{
				free(read);
				break ;
			}
			if (!check_empty_input(read))
				read_toks(head, read);
			free(read);
		}
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
