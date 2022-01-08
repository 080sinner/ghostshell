/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_control.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/27 20:11:59 by eozben            #+#    #+#             */
/*   Updated: 2022/01/08 19:21:25 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	print_error_type(t_node *node, t_tok *token)
{
	if (node && node->type)
	{
		if (node->type == PIPE)
			return (printf(" '|' \n"));
		if (node->type == AND)
			return (printf(" '&&' \n"));
		if (node->type == OR)
			return (printf(" '||' \n"));
	}
	else if (token && token->type)
	{
		if (token->type == GREAT)
			return (printf(" > \n"));
		if (token->type == GREATGREAT)
			return (printf(" >> \n"));
		if (token->type == LESS)
			return (printf(" < \n"));
		if (token->type == LESSLESS)
			return (printf(" << \n"));
	}
	return (1);
}

static int	check_syntax_controlop(t_node *current)
{
	t_node	*next;

	next = current->next;
	if (!current->previous)
	{
		printf("syntax error: missing command before control operator");
		return (print_error_type(current, NULL));
	}
	if (!next)
	{
		printf("syntax error: missing command after control operator");
		return (print_error_type(current, NULL));
	}
	if (next && next->type != LPAREN && next->type != COMMAND)
	{
		printf("syntax error: missing command after control operator");
		return (print_error_type(current, NULL));
	}
	return (0);
}

static int	check_syntax_command(t_node *node)
{
	t_tok	*current;
	t_tok	*next;

	current = node->args;
	next = current->next;
	while (current)
	{
		if (current->type != COMMAND && !current->next)
		{
			printf("syntax error: missing token after redirection operator");
			return (print_error_type(NULL, current));
		}
		else if (current->type != COMMAND && next && next->type != COMMAND)
		{
			printf("syntax error: wrong token after redirection operator");
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
				return (printf("syntax error: missing closing parentheses\n"));
		}
		else if (current->type == RPAREN)
		{
			if (!search_lparen(current, 0))
				return (printf("syntax error: missing opening parentheses\n"));
		}
		else if (current->type != COMMAND && check_syntax_controlop(current))
			return (1);
		else if (current->type == COMMAND && check_syntax_command(current))
			return (1);
		current = current->next;
	}
	return (0);
}
