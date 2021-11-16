/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 20:31:46 by eozben            #+#    #+#             */
/*   Updated: 2021/11/16 21:36:05 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_list(t_tok *head)
{
	while (head != NULL)
	{
		printf("%d\n", head->type);
		head = head->next;
	}
}

void	read_toks(t_tok *head)
{
	t_
}

int	main(void)
{
	char	*read;
	t_tok	*head;

	head = NULL;
	while (1)
	{
		read = readline("minish $ ");
		if (!ft_strcmp(read, "\0"))
			add_history(read);
		read_toks(head);
		free(read);
		if (ft_strcmp(read, "exit"))
			break ;
	}
	return (0);
}
