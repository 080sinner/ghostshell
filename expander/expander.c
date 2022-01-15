/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 19:53:36 by fbindere          #+#    #+#             */
/*   Updated: 2022/01/15 19:35:38 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_variable_expansion(t_tok **current, t_node *node)
{
	t_tok	*newlist;
	char	*tmp;
	int		i;

	i = 0;
	if (ft_strchr((*current)->data, END))
	{
		newlist = expand_variable((*current)->data, NULL, 42);
		if (!newlist)
		{
			tmp = (*current)->data;
			while (tmp[i] != END)
				i++;
			(*current)->data = ft_substr((*current)->data,
					i + 1, ft_strlen((*current)->data));
			ft_free((void *)&tmp, ft_strlen(tmp));
			return (ERROR);
		}
		insert_sublist(*current, newlist);
		free_tok(&node->args, *current);
		*current = newlist;
	}
	return (0);
}

int	expander(t_node *node)
{
	t_tok	*current;
	t_tok	*tmp;

	current = node->args;
	while (current)
	{
		if (current->data && check_variable_expansion(&current, node) == ERROR)
			continue ;
		if (node->here_doc && node->here_doc->state == FALSE)
		{
			if (expand_here_doc(node->here_doc) == -1)
				return (ERROR);
		}
		tmp = current->next;
		if (current->data && expand_wildcards(&current, &node->args) == ERROR)
			return (ERROR);
		current = tmp;
	}
	return (0);
}
