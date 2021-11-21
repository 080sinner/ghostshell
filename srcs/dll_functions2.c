/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dll_functions2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 18:58:36 by fbindere          #+#    #+#             */
/*   Updated: 2021/11/21 21:46:17 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_dll(t_tok **head)
{
	if ((*head) == NULL)
		return ;
	free(detach_tok(head, *head));
	free_dll(head);
}

void	free_list(t_tok *head)
{
	while (head != NULL)
	{
		free(head->data);
		head = head->previous;
	}
	free_dll(&head);
}

t_tok	*ft_dll_append_tok(t_tok **head)
{
	t_tok	*newtok;

	newtok = ft_calloc(1, sizeof(t_tok));
	if (!newtok)
	{
		free_dll(head);
		exit(EXIT_FAILURE);
	}
	newtok->next = NULL;
	newtok->previous = NULL;
	ft_dll_attach_tok(head, newtok);
	return (newtok);
}

t_node	*ft_dll_append_node(t_node **head)
{
	t_node	*newnode;

	newnode = ft_calloc(1, sizeof(t_node));
	// if (!newnode)
	// {
	// 	free_dll(head);
	// 	exit(EXIT_FAILURE);
	// }
	newnode->next = NULL;
	newnode->previous = NULL;
	ft_dll_attach_node(head, newnode);
	return (newnode);
}

int	check_whitespace(char c)
{
	if (c == ' ' || c == '\n' || c == '\t')
		return (1);
	return (0);
}
