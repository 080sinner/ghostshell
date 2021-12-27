/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/27 20:11:59 by eozben            #+#    #+#             */
/*   Updated: 2021/12/27 21:20:17 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_syntax_controlop(t_node *current)
{
	if (current->type == LPAREN || current->type == RPAREN)
		return (0);
	if (current && current->next
		&& current->type != COMMAND && current->next->type != COMMAND)
	{	
		printf("syntax error: too many control operators\n");
		return (1);
	}
	else if (current && current->type != COMMAND && !current->next)
	{
		printf("syntax error: missing command after control operator\n");
		return (1);
	}
	return (0);
}

int	check_syntax_command(t_tok *args)
{
	t_tok	*current;

	current = args;
	while (current)
	{
		if (current->type != COMMAND && !current->next)
		{
			printf("syntax error: missing token after redirection operator\n");
			return (1);
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
		if (check_syntax_controlop(current))
			return (1);
		if ((current->type == LPAREN && skip_paren_content(current, 0) == NULL)
			|| (current->type == RPAREN && !search_lparen(current)))
			return (printf("unequal amount of parentheses\n"));
		if (current->type == COMMAND && check_syntax_command(current->args))
			return (1);
		current = current->next;
	}
	return (0);
}
