/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 14:46:47 by eozben            #+#    #+#             */
/*   Updated: 2022/01/13 22:31:12 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	check_type(char *s)
{
	if (s[0] == LPAREN || s[0] == RPAREN)
		return (s[0]);
	else if (s[0] == PIPE || s[0] == AMPERSAND)
	{
		if (s[0] == s[1])
			return (s[0] * 4);
		return (s[0]);
	}
	else
		return (COMMAND);
}

int	check_state(char **input, int *state, t_tok *new)
{
	if (**input == SQUOTE && (*state == GENERAL_STATE
			|| *state == SQUOTED_STATE))
	{
		if (*state == GENERAL_STATE)
			*state = SQUOTED_STATE;
		else if (*state == SQUOTED_STATE)
			*state = GENERAL_STATE;
		new->state = TRUE;
		*input += 1;
		return (1);
	}
	if (**input == DQUOTE && (*state == GENERAL_STATE
			|| *state == DQUOTED_STATE))
	{
		if (*state == GENERAL_STATE)
			*state = DQUOTED_STATE;
		else if (*state == DQUOTED_STATE)
			*state = GENERAL_STATE;
		new->state = TRUE;
		*input += 1;
		return (1);
	}
	return (0);
}

int	is_control_op(t_token c)
{
	if (c == PIPE || c == LPAREN || c == RPAREN || c == AND || c == OR)
		return (1);
	return (0);
}

int	check_whitespace(char c)
{
	if (c == ' ' || c == '\n' || c == '\t')
		return (1);
	return (0);
}

t_token	is_redir_op(char *s)
{
	if (s[0] == GREAT || s[0] == LESS)
	{
		if (s[0] == s[1])
			return (s[0] * 4);
		return (s[0]);
	}
	else
		return (COMMAND);
}
