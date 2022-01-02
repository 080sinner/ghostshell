/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/27 20:11:59 by eozben            #+#    #+#             */
/*   Updated: 2022/01/02 22:59:21 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_syntax_controlop(t_node *current)
{
	if (!current->previous)
		return(printf("syntax error: expected command before control operator\n"));
	if (!current->next)
		return(printf("syntax error: expected command after control operator\n"));
	if (current->next && current->next->type != COMMAND)
		return(printf("syntax error: a control operator can not follow another control operator"));
	return (0);
}

int	check_syntax_command(t_tok *args, t_node *next_node)
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
		else if (current->type != COMMAND && next_node && next_node->type != COMMAND)
		{
			printf("syntax error: wrong token after redirection operator\n");
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
		if (current->type == LPAREN && skip_paren_content(current, 0) == NULL)
			return(printf("missing closing parentheses"));
		else if (current->type == RPAREN && !search_lparen(current))
			return(printf("missing opening parentheses"));
		else if (current->type != COMMAND && check_syntax_controlop(current))
			return (1);
		if (current->type == COMMAND && check_syntax_command(current->args, current->next))
			return (1);
		current = current->next;
	}
	return (0);
}
