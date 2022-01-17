/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 15:28:10 by fbindere          #+#    #+#             */
/*   Updated: 2022/01/17 18:01:49 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	new_node(int state, char *input)
{
	if (state == GENERAL_STATE && is_control_op(check_type(input)))
		return (1);
	return (0);
}

static int	get_word(char **input, t_tok *new, int *state)
{
	while (**input != '\0')
	{
		if (check_state(input, state, new))
			continue ;
		if (check_expansion(input, state))
		{
			if (mark_variable(input, new) == ERROR)
				return (ERROR);
			continue ;
		}
		if (*state == GENERAL_STATE && (check_whitespace(**input)
				|| is_control_op(check_type(*input)) || is_redir_op(*input)))
		{
			while (check_whitespace(**input))
				*input += 1;
			if (new_node(*state, *input))
				return (NEW_NODE);
			break ;
		}
		new->data = ft_append(new->data, **input);
		if (!new->data)
			return (ERROR);
		*input += 1;
	}
	return (0);
}

static int	read_command(char **input, t_node **command, int *state)
{
	t_tok	*new;
	int		ret;

	(*command)->args = NULL;
	while (**input != '\0' && !new_node(*state, *input))
	{
		new = create_new_tok();
		if (!new)
			return (ERROR);
		ft_dll_attach_tok(&(*command)->args, new);
		if (is_redir_op(*input))
		{
			new->type = is_redir_op(*input);
			*input += 1;
			if (new->type > 127)
				*input += 1;
			while (check_whitespace(**input))
				*input += 1;
			continue ;
		}
		ret = get_word(input, new, state);
		if (ret == ERROR || ret == NEW_NODE)
			return (ret);
	}
	return (0);
}

static int	read_input(t_node **head, char *input)
{
	t_node	*new;
	int		state;

	state = GENERAL_STATE;
	new = NULL;
	while (*input != '\0')
	{
		if (check_whitespace(*input))
		{
			input++;
			continue ;
		}
		new = ft_dll_append_node(head);
		if (!new)
			return (ERROR);
		new->type = check_type(input);
		if (new->type > 127)
			input += 2;
		else if (new->type != COMMAND)
			input++;
		else if (read_command(&input, &new, &state) == ERROR)
			return (ERROR);
	}
	return (state);
}

int	lexer(t_node **head, char *input)
{
	int		ret;

	ret = 0;
	ret = read_input(head, input);
	if (ret == ERROR)
		return (ERROR);
	else if (ret != GENERAL_STATE)
		return (ft_printnl_fd("spooky syntax: invalid use of quotes", 2));
	if (check_input(head))
		return (1);
	if (read_here_docs(head) == ERROR)
	{
		g_utils.exit_status = 1;
		return (ERROR);
	}
	return (0);
}
