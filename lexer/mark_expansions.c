/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mark_expansions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 20:02:59 by fbindere          #+#    #+#             */
/*   Updated: 2022/01/15 19:37:46 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_expansion(char **input, int *state)
{
	if (*state == GENERAL_STATE && **input == '*')
		**input = -42;
	if (*state != SQUOTED_STATE && **input == '$')
	{
		if (!ft_isalnum(*(*input + 1)) && *(*input + 1) != '_'
			&& *(*input + 1) != '?')
		{
			if (*state == SQUOTED_STATE && *(*input + 1) == SQUOTE)
				return (0);
			if (*state == DQUOTED_STATE && *(*input + 1) == DQUOTE)
				return (0);
		}
		if (*(*input + 1) == '\0')
			return (0);
		if (check_whitespace(*(*input + 1)))
			return (0);
		if (is_control_op(*(*input + 1)))
			return (0);
		**input = *state;
		return (1);
	}
	return (0);
}

int	mark_variable(char **input, t_tok *new)
{
	new->data = ft_append(new->data, **input);
	if (!new->data)
		return (ERROR);
	*input += 1;
	while (**input != '\0' && (ft_isalnum(**input) || **input == '_'))
	{
		new->data = ft_append(new->data, **input);
		if (!new->data)
			return (ERROR);
		*input += 1;
	}
	if (**input != '\0' && *(*input) == '?')
	{
		if (*(*input - 1) == DQUOTED_STATE || *(*input - 1) == GENERAL_STATE)
		{
			new->data = ft_append(new->data, **input);
			if (!new->data)
				return (ERROR);
			*input += 1;
		}			
	}
	new->data = ft_append(new->data, END);
	if (!new->data)
		return (ERROR);
	return (0);
}
