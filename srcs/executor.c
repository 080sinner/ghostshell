/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 17:44:02 by fbindere          #+#    #+#             */
/*   Updated: 2021/12/13 21:20:00 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


// 1. expand command
// 2. read input/output for command and remove red op.
// 3. aus liste char** createn,
// 3. 

// find_pipeline (t_node **head)
// {
// 	t_node *current;
// 	int		child_count;
// 	int		parentheses_count;

// 	child_count = 0;
// 	parentheses_count = 0;
// 	while (current)
// 	{
// 		if (current->type == COMMAND && current->next->type == PIPE)
// 		{
// 			child_count++;
// 			current = current->next;
// 		}
// 		else if (current->type == LPAREN)
// 		{
// 			parentheses_count++;
// 			while (current->type != RPAREN)
// 				current = current->next;
// 			if (current->type == RPAREN)
// 				parentheses_count--;
// 			if (parentheses_count != 0)
// 				printf("gib richtige klammern ein du spast\n");
// 		}
// 		current = current->next;
// 	}
// }
// ?

find_pipeline (t_node **head)
{
	t_node current;
	int command_count;

	command_count = 0;
	while (current)
	{
		if (current->type == COMMAND)
			command_count++;
		if (current->type == OR || current->type == AND)
			break;
		if (curent->type == LPAREN)
		{
			current = skip_paren_content(current);
		}
	}
}

t_node	*skip_paren_content(t_node *current)
{
	static int parencount;

	if (current->type == LPAREN)
		parencount++;
	if (current->type == RPAREN)
		parencount--;
	if (parencount == 0)
		return (current);
	return (skip_paren_content(current->next));
}

void	executor(t_node **head, t_node *end_of_loop)
{
	t_node *current;

	current = *head;
	while (current != end_of_loop)
	{
		while (current->type != OR && current->type != AND)
		{
			if (current->type == COMMAND)
				printf("command:%s\n", current->args->data);
				//execute_command (current);
			if (current->type == LPAREN)
				executor(&current, skip_paren_content(current));
			current = current->next;
		}
		if (current->type == OR || current->type == AND)
		{
			if (current->type == OR)
				printf("OR\n");
			else
				printf("AND\n");
			//check_exit_status(current);
		}
		current = current->next;
	}
}


execut
