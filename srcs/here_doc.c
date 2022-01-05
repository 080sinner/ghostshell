/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 20:57:30 by fbindere          #+#    #+#             */
/*   Updated: 2022/01/05 20:26:42 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*combine_strings(char *front, char *variable, char *end)
{
	char	*temp[2];
	char	*new;

	if (ft_getenv(variable, g_utils.environment) != NULL)
		temp[1] = ft_strdup(ft_getenv(variable, g_utils.environment));
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

int		expand_here_doc(t_tok *here_doc)
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
	return (0);
}

char	*convert_variable_delimiter(char *data)
{
	char	*new_var;
	int		counter[2];

	new_var = ft_calloc(ft_strlen(data), sizeof(char));
	if (!new_var)
	{
		ft_free((void *)&data, ft_strlen(data));
		return (NULL);
	}
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

int	here_doc(t_node *command, t_tok *here_doc)
{
	char	*line;
	t_tok	*new;

	line = NULL;
	if (!here_doc->next)
		return (0);
	if (here_doc->next->data && ft_strchr(here_doc->next->data, END))
		here_doc->next->data = convert_variable_delimiter(here_doc->next->data);
	if (!here_doc->next->data)
		return (ERROR);
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
		new = ft_dll_append_tok(&(command->here_doc));
		if (!new)
			return (ERROR);
		new->data = line;
		new->state = here_doc->next->state;
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
				if (here_doc(curr_node, curr_tok) == ERROR)
					return (ERROR);
			}
			curr_tok = curr_tok->next;
		}
		curr_node = curr_node->next;
	}
	return (0);
}
