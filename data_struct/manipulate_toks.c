/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manipulate_toks.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 20:41:00 by eozben            #+#    #+#             */
/*   Updated: 2022/01/06 00:43:20 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_dll_insert_tok(t_tok **head, t_tok *insert)
{
	if (*head == NULL)
	{
		*head = insert;
		insert->next = NULL;
		insert->previous = NULL;
	}
	else if ((*head)->next == NULL)
		ft_dll_attach_tok(head, insert);
	else
	{
		(*head)->next->previous = insert;
		insert->next = (*head)->next;
		(*head)->next = insert;
		insert->previous = *head;
	}
}

void	insert_sublist(t_tok *slot, t_tok *insert)
{
	if (slot == NULL || insert == NULL)
		return ;
	if (slot->next != NULL)
		slot->next->previous = ft_last_tok(insert);
	ft_last_tok(insert)->next = slot->next;
	slot->next = insert;
	insert->previous = slot;
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

t_tok	*ft_last_tok(t_tok *head)
{
	if (head == NULL)
		return (NULL);
	while (head->next != NULL)
		head = head->next;
	return (head);
}
