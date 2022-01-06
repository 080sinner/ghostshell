/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_here_doc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 20:57:30 by fbindere          #+#    #+#             */
/*   Updated: 2022/01/06 01:27:32 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*convert_variable_delimiter(char *data)
{
	char	*new_var;
	int		counter[2];

	new_var = ft_calloc(ft_strlen(data), sizeof(char));
	if (!new_var)
		return (ft_free((void *)&data, ft_strlen(data)));
	while (data && ft_strchr(data, GENERAL_STATE))
		*(ft_strchr(data, GENERAL_STATE)) = '$';
	while (data && ft_strchr(data, DQUOTED_STATE))
		*(ft_strchr(data, DQUOTED_STATE)) = '$';
	counter[0] = 0;
	counter[1] = 0;
	while (data[counter[0]])
	{
		if (data[counter[0]] != END)
		{
			new_var[counter[1]] = data[counter[0]];
			counter[1]++;
		}
		counter[0]++;
	}
	ft_free((void *)&data, ft_strlen(data));
	return (new_var);
}

static int	here_doc(t_node *command, t_tok *delim)
{
	char	*line;
	t_tok	*new;

	if (!delim)
		return (0);
	if (delim->data && ft_strchr(delim->data, END))
		delim->data = convert_variable_delimiter(delim->data);
	if (!delim->data)
		return (ERROR);
	while (1)
	{
		line = readline(">");
		if (ft_strcmp(line, delim->data) || delim->type != COMMAND || !line)
		{
			if (line != NULL)
				free(line);
			break ;
		}
		new = ft_dll_append_tok(&(command->here_doc));
		if (!new)
			return (ERROR);
		new->data = line;
		new->state = delim->state;
	}
	return (0);
}

int	read_here_docs(t_node **head)
{
	t_node	*curr_node;
	t_tok	*curr_tok;

	curr_node = *head;
	while (curr_node)
	{
		curr_tok = curr_node->args;
		while (curr_tok)
		{
			if (curr_tok->type == LESSLESS)
			{
				if (curr_node->here_doc)
					free_toks(&(curr_node->here_doc));
				if (here_doc(curr_node, curr_tok->next) == ERROR)
					return (ERROR);
			}
			curr_tok = curr_tok->next;
		}
		curr_node = curr_node->next;
	}
	return (0);
}
