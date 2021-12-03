/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dll_nodes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 14:50:02 by eozben            #+#    #+#             */
/*   Updated: 2021/11/30 20:36:44 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_node	*ft_last_node(t_node *head)
{
	if (head == NULL)
		return (NULL);
	while (head->next != NULL)
		head = head->next;
	return (head);
}

t_node	*detach_node(t_node **head, t_node *node)
{
	if (head == NULL || *head == NULL)
		return (NULL);
	if ((*head)->next == NULL && (*head)->previous == NULL )
		*head = NULL;
	else if (node->previous == NULL)
	{
		*head = (*head)->next;
		(*head)->previous = NULL;
	}
	else if (node->next == NULL)
		node->previous->next = NULL;
	else
	{
		node->next->previous = node->previous;
		node->previous->next = node->next;
	}
	return (node);
}

void	ft_dll_attach_node(t_node **head, t_node *attachment)
{
	if (*head == NULL)
		(*head) = attachment;
	else
	{
		attachment->previous = ft_last_node(*head);
		ft_last_node(*head)->next = attachment;
	}
}

int	free_nodes(t_node **head)
{
	if ((*head) == NULL)
		return (1);
	if ((*head)->args != NULL)
		free_toks(&(*head)->args);
	free(detach_node(head, *head));
	free_nodes(head);
	return (1);
}

t_node	*ft_dll_append_node(t_node **head)
{
	t_node	*newnode;

	newnode = ft_calloc(1, sizeof(t_node));
	if (!newnode)
		exit(free_nodes(head));
	newnode->next = NULL;
	newnode->previous = NULL;
	ft_dll_attach_node(head, newnode);
	return (newnode);
}
