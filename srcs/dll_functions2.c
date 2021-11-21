/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dll_functions2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 18:58:36 by fbindere          #+#    #+#             */
/*   Updated: 2021/11/21 23:31:03 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_toks(t_tok **head)
{
	if ((*head) == NULL)
		return ;
	if ((*head)->data != NULL)
		free((*head)->data);
	free(detach_tok(head, *head));
	free_toks(head);
}

void	free_nodes(t_node **head)
{
	t_node	*tmp;

	if ((*head) == NULL)
		return ;
	while ((*head) != NULL)
	{
		free_toks(&(*head)->args);
		tmp = *head;
		*head = (*head)->next;
		free(tmp);
	}
}

t_tok	*ft_dll_append_tok(t_tok **head)
{
	t_tok	*newtok;

	newtok = ft_calloc(1, sizeof(t_tok));
	// if (!newtok)
	// {
	// 	free_dll(head);
	// 	exit(EXIT_FAILURE);
	// }
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
