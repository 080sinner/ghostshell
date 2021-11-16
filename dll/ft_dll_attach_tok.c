/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dll_attach_tok.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 13:26:31 by fbindere          #+#    #+#             */
/*   Updated: 2021/11/16 20:24:46 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
