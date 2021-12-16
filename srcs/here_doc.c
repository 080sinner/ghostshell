/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 20:57:30 by fbindere          #+#    #+#             */
/*   Updated: 2021/12/16 21:05:37 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*combine_strings(char *front, char *variable, char *end)
{
	char	*temp[2];
	char	*new;

	if (getenv(variable) != NULL)
		temp[1] = ft_strdup(getenv(variable));
	else
		temp[1] = ft_strdup("");
	ft_free((void *)&variable, ft_strlen(variable));
	temp[0] = ft_strjoin(front, temp[1]);
	ft_free((void *)&temp[1], ft_strlen(temp[1]));
	ft_free((void *)&front, ft_strlen(front));
	new = ft_strjoin(temp[0], end);
	free(temp[0]);
	free(end);
	return (new);
}

int	contains_variable(char *string)
{
	int	i;

	i = 0;
	while (string[i] != '\0')
	{
		if (string[i] == '$' && (ft_isalnum(string[i + 1]) 
				|| string[i + 1] == '_'))
			return (1);
		i++;
	}
	return (0);
}

void	expand_here_doc(t_tok *here_doc)
{
	char	*front;
	char	*variable;
	char	*end;
	t_tok 	*current;
	int		i;
	int		varlen;

	current = here_doc;
	while (current)
	{
		while (contains_variable(current->data))
		{
			i = 0;
			while (current->data[i] != '$')
				i++;
			varlen = i;
			while (current->data[i] == '$')
				i++;
			i = varlen;
			front = ft_substr(current->data, 0, i);
			i++;
			varlen = 0;
			while (ft_isalnum(current->data[i]) || current->data[i] == '_')
			{
				i++;
				varlen++;
			}
			variable = ft_substr(ft_strchr(current->data, '$'), 1, varlen);
			end = ft_substr(current->data, i, ft_strlen(current->data) - 1);
			free(current->data);
			current->data = combine_strings(front, variable, end);
		}
		current = current->next;
	}
}


int	here_doc(t_node *command, t_tok *here_doc, t_node **head)
{
	char	*line;
	t_tok	*new;

	line = NULL;
	if (!here_doc->next)
		return (0);
	while (1)
	{
		line = readline(">");
		if (ft_strcmp(line, here_doc->next->data)
			|| here_doc->next->type != COMMAND || line == NULL)
		{
			if (line != NULL)
				free(line);
			break ;
		}
		new = ft_dll_append_tok(&(command->here_doc), head);
		new->data = line;
		new->state = here_doc->next->state;
	}
	return (0);
}

void	read_here_docs(t_node **head)
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
				here_doc(curr_node, curr_tok, head);
			}
			curr_tok = curr_tok->next;
		}
		curr_node = curr_node->next;
	}
}
