/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dll_insert_tok.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 13:06:40 by fbindere          #+#    #+#             */
/*   Updated: 2021/11/16 13:50:52 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	ft_dll_insert_node(t_tok *head, t_tok *attachment)
{
	if (head == NULL)
	{
		head = attachment;
		attachment->next = NULL;
		attachment->previous = NULL;
	}
	else
	{
		head->next->previous = attachment;
		attachment->next = head->next;
		head->next = attachment;
		attachment->previous = head;
	}
}