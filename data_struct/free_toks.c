/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_toks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 14:53:53 by eozben            #+#    #+#             */
/*   Updated: 2022/01/06 00:43:17 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	free_toks(t_tok **head)
{
	if ((*head) == NULL)
		return (ERROR);
	if ((*head)->data != NULL)
		ft_free((void *)&(*head)->data, ft_strlen((*head)->data));
	free(detach_tok(head, *head));
	free_toks(head);
	return (ERROR);
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

void	free_tok(t_tok **head, t_tok *tok)
{
	if (tok->data)
		ft_free((void *)&tok->data, ft_strlen(tok->data));
	free(detach_tok(head, tok));
}
