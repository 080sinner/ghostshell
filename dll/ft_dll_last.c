/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dll_last.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 13:28:57 by fbindere          #+#    #+#             */
/*   Updated: 2021/11/16 13:43:05 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

t_tok *ft_last_element(t_tok *head)
{
	if (head == NULL)
		return (NULL);
	while(head->next != NULL)
		head = head->next;
	return(head);
}
