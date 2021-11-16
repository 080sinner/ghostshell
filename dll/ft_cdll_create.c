/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cdll_create.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 18:58:36 by fbindere          #+#    #+#             */
/*   Updated: 2021/11/16 14:11:03 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

t_node	*ft_cdll_create_node(t_tok *head)
{
	t_tok	*newtok;

	newtok = ft_calloc(1, sizeof(t_tok));
	if (newtok == NULL)
		ft_free_dll(head);
	ft_cdll_attach_tok(head, newtok);
	return (newtok);
}
