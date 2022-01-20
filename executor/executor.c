/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/18 17:09:14 by fbindere          #+#    #+#             */
/*   Updated: 2022/01/20 23:48:59 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	exit_status(t_exec *exec)
{
	int	stat[2];

	while (1)
	{
		stat[EXIT] = waitpid(-1, &stat[LOG], 0);
		if (exec->pid == stat[EXIT])
		{
			if (WIFEXITED(stat[LOG]))
				g_utils.exit_status = WEXITSTATUS(stat[LOG]);
			if (WIFSIGNALED(stat[LOG]))
			{
				if (WTERMSIG(stat[LOG]) == SIGQUIT)
				{
					ft_putstr_fd("Quit: ", 2);
					ft_putnbr_fd(SIGQUIT, 2);
					ft_putendl_fd("", 2);
				}
				else if (WTERMSIG(stat[LOG]) == SIGINT)
					ft_putendl_fd("", 2);
				g_utils.exit_status = 127 + WTERMSIG(stat[LOG]) + 1;
			}
		}
		if (stat[EXIT] == ERROR)
			break ;
	}
}

static t_node	*skip_pipeline(t_node *command)
{
	static int	count;

	command = command->next;
	if (!command)
		return (command);
	if (command->type == LPAREN)
		count++;
	if (command->type == RPAREN)
		count--;
	if (count == 0 && (command->type == OR || command->type == AND))
		return (command);
	else
		return (skip_pipeline(command));
}

static int	logic_operator(t_node *current)
{
	if (current && current->type == OR && g_utils.exit_status == 0)
		return (1);
	if (current && current->type == AND && g_utils.exit_status != 0)
		return (1);
	return (0);
}

void	executor(t_node *current, int process_lvl, t_node **head)
{
	t_exec	exec;

	init_exec(&exec, process_lvl, head);
	while (1)
	{
		if (current && (current->type == COMMAND || current->type == LPAREN))
			execute_command(&exec, &current, head);
		if (!current || current->type == OR || current->type == AND)
		{
			ft_close(exec.tmp_fd, "executor", head, NO_EXIT);
			exec.tmp_fd = ft_dup(STDIN_FILENO, "executor", head, NO_EXIT);
			exit_status(&exec);
		}
		if (logic_operator(current))
		{
			current = skip_pipeline(current);
			continue ;
		}
		if (!current)
			break ;
		current = current->next;
	}
}
