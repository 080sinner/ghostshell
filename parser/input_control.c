/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_control.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/27 20:11:59 by eozben            #+#    #+#             */
/*   Updated: 2022/01/20 19:02:54 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	check_syntax_controlop(t_node *current)
{
	t_node	*next;

	next = current->next;
	if (!current->previous)
	{
		ft_putstr_fd("Spooky syntax: missing command before control operator",
			2);
		return (print_error_type(current, NULL));
	}
	if (!next)
	{
		ft_putstr_fd("Spooky syntax: missing command after control operator",
			2);
		return (print_error_type(current, NULL));
	}
	if (next && next->type != LPAREN && next->type != COMMAND)
	{
		ft_putstr_fd("Spooky syntax: missing command after control operator",
			2);
		return (print_error_type(current, NULL));
	}
	return (0);
}

static int	check_syntax_command(t_node *node)
{
	t_tok	*current;
	t_tok	*next;

	if (node->next && (node->next->type == LPAREN))
		return (ft_printnl_fd(
				"Spooky syntax: Missing control operator before command", 2));
	current = node->args;
	while (current)
	{
		next = current->next;
		if (current->type != COMMAND && !current->next)
		{
			ft_putstr_fd("Spooky syntax: missing token after ", 2);
			ft_putstr_fd("redirection operator", 2);
			return (print_error_type(NULL, current));
		}
		else if (current->type != COMMAND && next && next->type != COMMAND)
		{
			ft_putstr_fd("Spooky syntax: wrong token after ", 2);
			ft_putstr_fd("redirection operator", 2);
			return (print_error_type(NULL, current));
		}
		current = current->next;
	}
	return (0);
}

int	check_syntax_paren(t_node *current, t_node *partner_paren)
{
	if (current->type == LPAREN)
	{
		partner_paren = skip_paren_content(current, 0);
		if (!partner_paren)
			return (ft_printnl_fd("Spooky syntax: missing ')'", 2));
		if (partner_paren && current->next && partner_paren == current->next)
			return (ft_printnl_fd(
					"Spooky syntax: missing command in parentheses", 2));
		if (partner_paren->next && (partner_paren->next->type == COMMAND
				|| partner_paren->next->type == LPAREN))
			return (ft_printnl_fd(
					"Spooky syntax: missing operator after ')'", 2));
	}
	if (current->type == RPAREN)
	{
		partner_paren = search_lparen(current, 0);
		if (!partner_paren)
			return (ft_printnl_fd("Spooky syntax: missing '('", 2));
		if (current->next && (current->next->type == COMMAND
				|| current->next->type == LPAREN))
			return (ft_printnl_fd(
					"Spooky syntax: missing operator after ')'", 2));
	}
	return (0);
}

int	check_input(t_node **head)
{
	t_node	*current;

	current = *head;
	while (current)
	{
		if (current->type == LPAREN)
		{
			if (check_syntax_paren(current, NULL))
				return (1);
		}
		else if (current->type == RPAREN)
		{
			if (check_syntax_paren(current, NULL))
				return (1);
		}
		else if (current->type != COMMAND && check_syntax_controlop(current))
			return (1);
		else if (current->type == COMMAND && check_syntax_command(current))
			return (1);
		current = current->next;
	}
	return (0);
}
