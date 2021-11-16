/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dll_insert_tok.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 13:06:40 by fbindere          #+#    #+#             */
/*   Updated: 2021/11/16 16:40:40 by eozben           ###   ########.fr       */
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
