/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 20:31:46 by eozben            #+#    #+#             */
/*   Updated: 2021/11/16 19:17:13 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// void	ignore_signals(void)
// {
// 	signal(SIGINT, SIG_IGN);
// 	signal(SIGTSTP, SIG_IGN);
// 	signal(SIGQUIT, SIG_IGN);
// }

// int	main(void)
// {
// 	char	*read;

// 	while (1)
// 	{
// 		read = readline("minish $ ");
// 		if (!ft_strcmp(read, "\0"))
// 			add_history(read);
// 		free(read);
// 		if (ft_strcmp(read, "exit"))
// 			break ;
// 	}
// 	return (0);
// }

void	print_list(t_tok *head)
{
	head = ft_last_element(head);
	while (head != NULL)
	{
		printf("%d\n", head->type);
		head = head->previous;
	}
}

int	main(void)
{
	t_tok	*head;

	head = NULL;
	for (int i = 0; i<10; i++)
	{
		ft_dll_append_tok(&head);
		ft_last_element(head)->type = i;
	}
	detach_tok(&head, head->next);
	print_list(head);
	return (0);
}
