/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dll_toks.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 14:53:53 by eozben            #+#    #+#             */
/*   Updated: 2021/12/14 17:46:23 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	free_toks(t_tok **head)
{
	if ((*head) == NULL)
		return (-1);
	if ((*head)->data != NULL)
		free((*head)->data);
	free(detach_tok(head, *head));
	free_toks(head);
	return (-1);
}

t_tok	*ft_dll_append_tok(t_tok **head, t_node **head_node)
{
	t_tok	*newtok;

	newtok = ft_calloc(1, sizeof(t_tok));
	if (!newtok)
		exit(free_nodes(head_node));
	newtok->next = NULL;
	newtok->previous = NULL;
	newtok->state = FALSE;
	newtok->type = COMMAND;
	ft_dll_attach_tok(head, newtok);
	return (newtok);
}

t_tok	*ft_last_tok(t_tok *head)
{
	if (head == NULL)
		return (NULL);
	while (head->next != NULL)
		head = head->next;
	return (head);
}

t_tok	*detach_tok(t_tok **head, t_tok *node)
{
	if (head == NULL || *head == NULL)
		return (NULL);
	if ((*head)->next == NULL && (*head)->previous == NULL )
		*head = NULL;
	else if (node->previous == NULL)
	{
		*head = (*head)->next;
		(*head)->previous = NULL;
	}
	else if (node->next == NULL)
		node->previous->next = NULL;
	else
	{
		node->next->previous = node->previous;
		node->previous->next = node->next;
	}
	return (node);
}

void	ft_dll_attach_tok(t_tok **head, t_tok *attachment)
{
	if (*head == NULL)
		(*head) = attachment;
	else
	{
		attachment->previous = ft_last_tok(*head);
		ft_last_tok(*head)->next = attachment;
	}
}
