/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mac <mac@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 20:47:32 by fbindere          #+#    #+#             */
/*   Updated: 2021/11/25 06:22:44 by mac              ###   ########.fr       */
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
				printf("%s ", head->args->data);
			head->args = head->args->next;
		}
		if (head->type != COMMAND)
			printf("%c", head->type);
		printf("\n");
		head = head->next;
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

void	print_header(void)
{
	printf("\033[1;36m   _____ ________         .__       .__       .__    ");
	printf("       .__  .__  \n");
	printf("  /  |  |\\_____  \\  _____ |__| ____ |__| _____|  |__   ____");
	printf(" |  | |  |  \n");
	printf(" /   |  |_/  ____/ /     \\|  |/    \\|  |/  ___/  |  \\_/ __ ");
	printf("\\|  | |  |  \n");
	printf("/    ^   /       \\|  Y Y  \\  |   |  \\  |\\___ \\|   Y  ");
	printf("\\  ___/|  |_|  |_\n");
	printf("\\____   |\\_______ \\__|_|  /__|___|  /__/____  >___|  ");
	printf("/\\___  >____/____/\n");
	printf("     |__|        \\/     \\/        \\/        \\/     \\/     \\");
	printf("/                                                      \n\n\n");
	printf("\033[0m");
}

void	get_input(t_node **head)
{
	char	*read;

	print_header();
	while (1)
	{
		read = readline("	\033[1;36m༼ つ ▀_▀ ༽つ\033[0m	");
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
