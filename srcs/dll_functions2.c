/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dll_functions2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 18:58:36 by fbindere          #+#    #+#             */
/*   Updated: 2021/11/19 00:36:21 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_dll(t_tok **head)
{
	if ((*head) == NULL)
		return ;
	free(detach_tok(head, *head));
	free_dll(head);
}

t_tok	*ft_dll_append_tok(t_tok **head)
{
	t_tok	*newtok;

	newtok = ft_calloc(1, sizeof(t_tok));
	if (!newtok)
	{
		free_dll(head);
		exit(EXIT_FAILURE);
	}
	newtok->next = NULL;
	newtok->previous = NULL;
	ft_dll_attach_tok(head, newtok);
	return (newtok);
}

void	skip_whitespace(char **input)
{
	while (**input == ' ' || **input == '\n' || **input == '\t')
		*input += 1;
}
