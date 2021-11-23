/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>                    +#+  +:+       +#+   */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 14:46:47 by eozben            #+#    #+#             */
/*   Updated: 2021/11/23 01:01:36 by eozben              ###   ########.fr     */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	check_type(char *s)
{
	if (s[0] == LPAREN || s[0] == RPAREN || s[0] == SPACE || s[0] == TAB
		|| s[0] == NEWLINE || s[0] == VARIABLE)
		return (s[0]);
	else if (s[0] == PIPE || s[0] == AMPERSAND || s[0] == GREAT || s[0] == LESS)
	{
		if (s[0] == s[1])
			return (s[0] * 4);
		return (s[0]);
	}
	else
		return (COMMAND);
}

int	check_state(char c, int *state)
{
	if (c == SQUOTE && (*state == GENERAL_STATE || *state == SQUOTED_STATE))
	{
		if (*state == GENERAL_STATE)
			*state = SQUOTED_STATE;
		else if (*state == SQUOTED_STATE)
			*state = GENERAL_STATE;
		return (1);
	}
	if (c == DQUOTE && (*state == GENERAL_STATE || *state == DQUOTED_STATE))
	{
		if (*state == GENERAL_STATE)
			*state = DQUOTED_STATE;
		else if (*state == DQUOTED_STATE)
			*state = GENERAL_STATE;
		return (1);
	}
	return (0);
}

int	check_ctrlop_whitespace(int state, char **input)
{
	if (state == GENERAL_STATE && (check_whitespace(**input)
			|| is_control_op(check_type(*input)) == TRUE))
	{
		while (check_whitespace(**input))
			*input += 1;
		if (is_control_op(check_type(*input)) == TRUE)
			return (NEW_NODE);
		return (NEW_TOK);
	}
	return (0);
}

int	is_control_op(t_token c)
{
	if (c == PIPE || c == LPAREN || c == RPAREN || c == GREAT || c == LESS
		|| c == AND || c == OR || c == LESSLESS || c == GREATGREAT)
		return (1);
	return (0);
}

int	check_whitespace(char c)
{
	if (c == ' ' || c == '\n' || c == '\t')
		return (1);
	return (0);
}
