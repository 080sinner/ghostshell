/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dll_functions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 20:41:00 by eozben            #+#    #+#             */
/*   Updated: 2021/11/18 21:27:15 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_tok	*ft_last_element(t_tok *head)
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

void	insert_sublist(t_tok *slot, t_tok *insert)
{
	if (slot == NULL || insert == NULL)
		return ;
	slot->next->previous = ft_last_element(insert);
	ft_last_element(insert)->next = slot->next;
	slot->next = insert;
	insert->previous = slot;
}

void	ft_dll_attach_tok(t_tok **head, t_tok *attachment)
{
	if (*head == NULL)
		(*head) = attachment;
	else
	{
		attachment->previous = ft_last_element(*head);
		ft_last_element(*head)->next = attachment;
	}
}

void	ft_dll_insert_tok(t_tok **head, t_tok *insert)
{
	if (*head == NULL)
	{
		*head = insert;
		insert->next = NULL;
		insert->previous = NULL;
	}
	else
	{
		(*head)->next->previous = insert;
		insert->next = (*head)->next;
		(*head)->next = insert;
		insert->previous = *head;
	}
}
