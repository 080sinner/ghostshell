/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 23:18:50 by eozben            #+#    #+#             */
/*   Updated: 2021/12/03 20:17:15 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	get_variable_word(t_tok *current, char *var_value)
{
	t_tok	*new;

	new = current;
	while (*var_value != '\0')
	{
		if (check_whitespace(*var_value))
		{
			new = ft_dll_append_tok(&new);
			new->data = ft_strdup("");
			if (!new)
				return (free_toks(&new));
		}
		if (*var_value == '*')
			*var_value = -42;
		new->data = ft_append(new->data, *var_value);
		var_value++;
	}
	return (0);
}

int	dquoted_variable(int state, char **variable, t_tok *new)
{
	if (state == DQUOTED_STATE && variable[VAR_VALUE] != NULL)
	{
		variable[TMP] = new->data;
		new->data = ft_strjoin(new->data, variable[VAR_VALUE]);
		if (!new->data)
			return (free_toks(&new));
		free(variable[TMP]);
	}
	return (0);
}

int	get_var_name(char **input, int *i)
{
	
	
	
	
	
	*input += 1;
	*i = 0;
	while (ft_isalnum((*input)[*i]) || (*input)[*i] == '_')
		*i += 1;
	if (**input == '?')
	{
		**input = -69;
		return (0);
	}
	return (1);
}

int	expand_variable(t_tok *token)
{
	int		i;
	char	*variable[2];

	i = 0;
	if (!get_var_name(token->data, &i))
		return (0);
	variable[TMP] = ft_substr(*input, 0, i);
	if (!variable[TMP])
		return (free_toks(&new));
	*input += i;
	variable[VAR_VALUE] = getenv(variable[TMP]);
	ft_free((void *)&variable[TMP], ft_strlen(variable[TMP]));
	if (dquoted_variable(state, variable, new))
		return (-1);
	if (state == GENERAL_STATE && variable[VAR_VALUE] != NULL)
		return (get_variable_word(new, variable[VAR_VALUE]));
	return (0);
}

