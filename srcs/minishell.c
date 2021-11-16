/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 20:31:46 by eozben            #+#    #+#             */
/*   Updated: 2021/11/16 20:37:24 by fbindere         ###   ########.fr       */
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

int	main(void)
{
	t_tok	*head;
	t_tok	*test;

	head = NULL;
	test = NULL;
	for (int i = 0; i<10; i++)
	{
		ft_dll_append_tok(&head);
		ft_dll_append_tok(&test);
		ft_last_element(head)->type = i;
		ft_last_element(test)->type = i + 100;
	}
	//detach_tok(&head, head);
	print_list(head);
	free_dll(&head);
	free_dll(&test);
	return (0);
}
