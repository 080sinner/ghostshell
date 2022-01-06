/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_here_doc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/06 00:45:47 by fbindere          #+#    #+#             */
/*   Updated: 2022/01/06 21:44:26 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	contains_variable(char *string)
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

static void	expand_here_doc_variable(t_tok *current)
{
	int		i;
	int		varlen;
	char	*front;
	char	*variable;
	char	*end;

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

int	expand_here_doc(t_tok *here_doc)
{
	t_tok	*current;

	current = here_doc;
	while (current)
	{
		while (contains_variable(current->data))
			expand_here_doc_variable(current);
		current = current->next;
	}
	return (0);
}
