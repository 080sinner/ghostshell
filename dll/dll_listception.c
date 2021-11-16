/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dll_listception.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 16:44:55 by eozben            #+#    #+#             */
/*   Updated: 2021/11/16 17:34:13 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


t_tok	*detach_tok(t_tok **head, t_tok *node)
{
	if (head == NULL || *head == NULL)
		return (NULL);
	if ((*head)->next == NULL && (*head)->previous == NULL )
		*head = NULL;
	else
	{
		node->next->previous = node->previous;
		node->previous->next = node->next;
	}
	return (node);
}

// void	expand_list_into_list(t_tok *head)
// {
	
// }