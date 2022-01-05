/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dll_nodes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 14:50:02 by eozben            #+#    #+#             */
/*   Updated: 2022/01/05 20:10:23 by fbindere         ###   ########.fr       */
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
	else if (node && node->previous == NULL)
	{
		*head = (*head)->next;
		(*head)->previous = NULL;
	}
	else if (node && node->next == NULL)
		node->previous->next = NULL;
	else if (node)
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
	if ((*head)->here_doc != NULL)
		free_toks(&(*head)->here_doc);
	if ((*head)->cmdpath != NULL)
		ft_free((void *)&(*head)->cmdpath, ft_strlen((*head)->cmdpath));
	if ((*head)->cmd_arr != NULL)
		free((*head)->cmd_arr);
	free(detach_node(head, *head));
	free_nodes(head);
	return (1);
}

t_node	*ft_dll_append_node(t_node **head)
{
	t_node	*newnode;

	newnode = ft_calloc(1, sizeof(t_node));
	if (!newnode)
		return (NULL);
	newnode->here_doc = NULL;
	newnode->cmd_arr = NULL;
	newnode->cmdpath = NULL;
	newnode->args = NULL;
	newnode->next = NULL;
	newnode->previous = NULL;
	newnode->in = PIPEIN;
	newnode->out = PIPEOUT;
	ft_dll_attach_node(head, newnode);
	return (newnode);
}
