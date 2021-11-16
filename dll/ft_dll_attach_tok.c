/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dll_attach_tok.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 13:26:31 by fbindere          #+#    #+#             */
/*   Updated: 2021/11/16 18:04:46 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_dll_attach_tok(t_tok **head, t_tok *attachment)
{
	t_tok	*last_tok;

	if (*head == NULL)
		(*head) = attachment;
	else
	{
		last_tok = ft_last_element(*head);
		attachment->previous = last_tok;
		last_tok->next = attachment;
	}
}
