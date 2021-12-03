/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 20:57:30 by fbindere          #+#    #+#             */
/*   Updated: 2021/12/03 19:21:27 by fbindere         ###   ########.fr       */
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

char	*expand_here_doc(char *here_string)
{
	char	*front;
	char	*variable;
	char	*end;
	int		i;
	int		varlen;

	while (contains_variable(here_string))
	{
		i = 0;
		while (here_string[i] != '$')
			i++;
		varlen = i;
		while (here_string[i] == '$')
			i++;
		i = varlen;
		front = ft_substr(here_string, 0, i);
		varlen = i;
		i++;
		varlen = 0;
		while (ft_isalnum(here_string[i]) || here_string[i] == '_')
		{
			i++;
			varlen++;
		}
		variable = ft_substr(ft_strchr(here_string, '$'), 1, varlen);
		end = ft_substr(here_string, i, ft_strlen(here_string) - 1);
		free(here_string);
		here_string = combine_strings(front, variable, end);
	}
	return (here_string);
}

int	here_doc(t_node *here_doc_node, t_node **head)
{
	char	*line;
	t_tok	*new;

	line = NULL;
	while (1)
	{
		line = readline(">");
		if ((ft_strcmp(line, here_doc_node->next->args->data))
			|| here_doc_node->next->type != COMMAND || line == NULL)
		{
			if (line != NULL)
				free(line);
			break ;
		}
		new = ft_dll_append_tok(&(here_doc_node->args), head);
		// if (here_doc_node->next->args->state == GENERAL_STATE)
		// 	line = expand_here_doc(line);
		new->data = line;
	}
	return (0);
}
