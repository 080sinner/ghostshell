/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_control.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/27 20:11:59 by eozben            #+#    #+#             */
/*   Updated: 2022/01/11 21:12:40 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	print_error_type(t_node *node, t_tok *token)
{
	if (node && node->type)
	{
		if (node->type == PIPE)
			return (ft_printnl_fd(" '|' ", 2));
		if (node->type == AND)
			return (ft_printnl_fd(" '&&' ", 2));
		if (node->type == OR)
			return (ft_printnl_fd(" '||' ", 2));
	}
	else if (token && token->type)
	{
		if (token->type == GREAT)
			return (ft_printnl_fd(" > ", 2));
		if (token->type == GREATGREAT)
			return (ft_printnl_fd(" >> ", 2));
		if (token->type == LESS)
			return (ft_printnl_fd(" < ", 2));
		if (token->type == LESSLESS)
			return (ft_printnl_fd(" << ", 2));
	}
	return (1);
}

static int	check_syntax_controlop(t_node *current)
{
	t_node	*next;

	next = current->next;
	if (!current->previous)
	{
		ft_putstr_fd("spooky syntax : missing command before control operator",
			2);
		return (print_error_type(current, NULL));
	}
	if (!next)
	{
		ft_putstr_fd("spooky syntax : missing command after control operator",
			2);
		return (print_error_type(current, NULL));
	}
	if (next && next->type != LPAREN && next->type != COMMAND)
	{
		ft_putstr_fd("spooky syntax : missing command after control operator",
			2);
		return (print_error_type(current, NULL));
	}
	return (0);
}

static int	check_syntax_command(t_node *node)
{
	t_tok	*current;
	t_tok	*next;

	current = node->args;
	while (current)
	{
		next = current->next;
		if (current->type != COMMAND && !current->next)
		{
			ft_putstr_fd("spooky syntax : missing token after ", 2);
			ft_putstr_fd("redirection operator", 2);
			return (print_error_type(NULL, current));
		}
		else if (current->type != COMMAND && next && next->type != COMMAND)
		{
			ft_putstr_fd("spooky syntax : wrong token after ", 2);
			ft_putstr_fd("redirection operator", 2);
			return (print_error_type(NULL, current));
		}
		current = current->next;
	}
	return (0);
}

static t_node	*search_lparen(t_node *current, int first_call)
{
	static int	parencount;

	if (first_call == 0)
		parencount = 0;
	if (!current)
		return (NULL);
	if (current->type == RPAREN)
		parencount++;
	if (current->type == LPAREN)
		parencount--;
	if (parencount == 0)
		return (current);
	return (search_lparen(current->previous, ++first_call));
}

int	check_input(t_node **head)
{
	t_node	*current;

	current = *head;
	while (current)
	{
		if (current->type == LPAREN)
		{
			if (!skip_paren_content(current, 0))
				return (ft_printnl_fd("spooky syntax : missing closing \
						parentheses", 2));
		}
		else if (current->type == RPAREN)
		{
			if (!search_lparen(current, 0))
				return (ft_printnl_fd("syntax : missing opening parentheses",
						2));
		}
		else if (current->type != COMMAND && check_syntax_controlop(current))
			return (1);
		else if (current->type == COMMAND && check_syntax_command(current))
			return (1);
		current = current->next;
	}
	return (0);
}
