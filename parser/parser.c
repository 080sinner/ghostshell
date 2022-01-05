/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 17:27:51 by fbindere          #+#    #+#             */
/*   Updated: 2022/01/05 19:06:22 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	free_redir_op(t_tok **head, t_tok *node)
{
	if (!node)
		return ;
	if (node->next)
	{
		if (node->next->data)
			ft_free((void *)&node->next->data, ft_strlen(node->next->data));
		free(detach_tok(head, node->next));
	}
	if (node->data)
		ft_free((void *)&node->data, ft_strlen(node->data));
	free(detach_tok(head, node));
}

static int	set_output(t_node *command, t_tok *arg)
{
	if (arg->type == GREAT)
	{
		if (arg->next && arg->next->data)
			command->out = ft_open(arg->next->data, GREAT);
		free_redir_op(&command->args, arg);
	}
	else if (arg->type == GREATGREAT)
	{
		if (arg->next && arg->next->data)
			command->out = ft_open(arg->next->data, GREATGREAT);
		free_redir_op(&command->args, arg);
	}
	if (command->out == ERROR)
		return (ERROR);
	return (1);
}

static	int	set_input(t_node *command, t_tok *arg, t_node **head)
{
	if (arg->type == LESS)
	{
		if (arg->next && arg->next->data)
		{
			if (command->in != PIPEIN && command->in != STDIN_FILENO
				&& command->in != HERE_DOC)
				ft_close(command->in, "set_input", head, NO_EXIT);
			command->in = ft_open(arg->next->data, LESS);
		}
		free_redir_op(&command->args, arg);
		if (command->in == ERROR)
			return (ERROR);
	}
	else if (arg->type == LESSLESS)
	{
		command->in = HERE_DOC;
		free_redir_op(&command->args, arg);
	}
	return (1);
}

int	set_redir(t_node *command, t_node **head, t_node *prev, t_node *next)
{
	t_tok	*current;

	if (!command || !command->args)
		return (ERROR);
	current = command->args;
	if (!next || next->type == OR || next->type == AND)
		command->out = 1;
	if (!prev || prev->type == OR || prev->type == AND)
		command->in = STDIN_FILENO;
	while (current)
	{
		if (current->type == LESS || current->type == LESSLESS)
		{
			if (set_input(command, current, head) == ERROR)
				return (ERROR);
		}
		else if (current->type == GREAT || current->type == GREATGREAT)
		{
			if (set_output(command, current) == ERROR)
				return (ERROR);
		}
		current = current->next;
	}
	return (1);
}

int	parse_command(t_node *current, t_node **head)
{
	if (!current)
		return (ERROR);
	if (expander(current) == ERROR)
		return (ERROR);
	if (set_redir(current, head, current->previous, current->next) == ERROR)
		return (ERROR);
	if (create_array(current) == ERROR)
		return (ERROR);
	return (1);
}
