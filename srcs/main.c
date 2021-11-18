/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 20:47:32 by fbindere          #+#    #+#             */
/*   Updated: 2021/11/18 23:13:43 by eozben           ###   ########.fr       */
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
