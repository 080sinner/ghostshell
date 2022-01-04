/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/27 20:11:59 by eozben            #+#    #+#             */
/*   Updated: 2022/01/03 17:50:32 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int print_error_type (t_node *node, t_tok *token)
{
	if (node && node->type)
	{
		if (node->type == PIPE)
			return(printf(" '|' \n"));
		if (node->type == AND)
			return(printf(" '&&' \n"));
		if (node->type == OR)
			return(printf(" '||' \n"));
	}
	else if (token && token->type)
	{
		if (token->type == GREAT)
			return(printf(" > \n"));
		if (token->type == GREATGREAT)
			return(printf(" >> \n"));
		if (token->type == LESS)
			return(printf(" < \n"));
		if (token->type == LESSLESS)
			return(printf(" << \n"));
	}
	return (1);
}


int	check_syntax_controlop(t_node *current)
{
	if (!current->previous)
	{
		printf("syntax error: missing command before control operator");
		return(print_error_type(current, NULL));
	}
	if (!current->next || (current->next && current->next->type != COMMAND))
	{
		printf("syntax error: missing command after control operator");
		return(print_error_type(current, NULL));
	}
	return (0);
}

int	check_syntax_command(t_node *node)
{
	t_tok	*current;
	t_node	*next;
	
	current = node->args;
	next = node->next;
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

int	search_lparen(t_node *current)
{
	while (current)
	{
		if (current->type == LPAREN)
			return (1);
		current = current->previous;
	}
	return (0);
}

int	check_input(t_node **head)
{
	t_node	*current;

	current = *head;
	while (current)
	{
		if (current->type == LPAREN && !skip_paren_content(current, 0))
			return(printf("missing closing parentheses"));
		else if (current->type == RPAREN && !search_lparen(current))
			return(printf("missing opening parentheses"));
		else if (current->type != COMMAND && check_syntax_controlop(current))
			return (1);
		else if (current->type == COMMAND && check_syntax_command(current))
			return (1);
		current = current->next;
	}
	return (0);
}
