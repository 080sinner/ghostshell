/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 20:47:32 by fbindere          #+#    #+#             */
/*   Updated: 2021/11/18 22:43:32 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	print_list(t_tok *head)
{
	while (head != NULL)
	{
		//printf("%d\n", head->type);
		if(head->type == WORD)
			printf("%s\n", head->data);
		head = head->next;
	}
}

int	main(void)
{
	t_tok *head;
	head = NULL;
	char	*read;

	head = NULL;
	while (1)
	{
		read = readline("minish $ ");
		if (!ft_strcmp(read, "\0"))
			add_history(read);
		read_toks(&head, read);
		free(read);
		if (ft_strcmp(read, "exit"))
			break ;
	}
	print_list(head);
	return (0);
}