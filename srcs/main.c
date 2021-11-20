/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mac <mac@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 20:47:32 by fbindere          #+#    #+#             */
/*   Updated: 2021/11/20 05:03:06 by mac              ###   ########.fr       */
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

int	main(void)
{
	t_tok	*head;
	t_tok	*save;
	char	*read;

	head = NULL;
	while (1)
	{
		read = readline("minish $ ");
		if (!ft_strcmp(read, "\0"))
			add_history(read);
		save = read_toks(&head, read);
		if (ft_strcmp(read, "exit"))
		{
			free(read);
			break ;
		}
		free(read);
	}
	print_list(head);
	free_list(save);
	free_dll(&head);
	head = NULL;
	save = NULL;
	system("leaks minishell");
	return (0);
}
