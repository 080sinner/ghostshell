/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_toks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/06 00:15:55 by fbindere          #+#    #+#             */
/*   Updated: 2022/01/06 00:43:09 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_tok	*ft_dll_append_tok(t_tok **head)
{
	t_tok	*newtok;

	newtok = ft_calloc(1, sizeof(t_tok));
	if (!newtok)
		return (NULL);
	newtok->next = NULL;
	newtok->previous = NULL;
	newtok->state = FALSE;
	newtok->type = COMMAND;
	ft_dll_attach_tok(head, newtok);
	return (newtok);
}

t_tok	*create_new_tok(void)
{
	t_tok	*new;

	new = ft_calloc(1, sizeof(t_tok));
	if (!new)
		return (NULL);
	new->previous = NULL;
	new->next = NULL;
	new->state = FALSE;
	new->type = COMMAND;
	if (!new)
		return (NULL);
	new->data = ft_strdup("");
	if (!new->data)
	{
		free(new);
		return (NULL);
	}
	return (new);
}
