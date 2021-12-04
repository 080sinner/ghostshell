/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dll_functions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 20:41:00 by eozben            #+#    #+#             */
/*   Updated: 2021/12/04 19:34:52 by fbindere         ###   ########.fr       */
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
