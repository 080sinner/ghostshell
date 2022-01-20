/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 17:31:42 by fbindere          #+#    #+#             */
/*   Updated: 2022/01/20 18:26:04 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	child(t_exec *exec, t_node *command, t_node **head)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (parse_command(command, head) == ERROR || get_cmd_path(command) == ERROR)
		ft_exit(g_utils.exit_status, head);
	if (command->in == PIPEIN)
		ft_dup2(exec->tmp_fd, STDIN_FILENO, head, EXIT);
	else if (command->in == HERE_DOC)
		retrieve_here_doc(command, head);
	else
		ft_dup2(command->in, STDIN_FILENO, head, EXIT);
	if (command->out == PIPEOUT)
		ft_dup2(exec->pipe[1], STDOUT_FILENO, head, EXIT);
	else
		ft_dup2(command->out, STDOUT_FILENO, head, EXIT);
	ft_close(exec->pipe[0], "child", head, EXIT);
	ft_close(exec->pipe[1], "child", head, EXIT);
	ft_close(exec->tmp_fd, "child", head, EXIT);
	if (check_builtin(command->args))
	{
		execute_builtin(command, FALSE, head);
		ft_exit(g_utils.exit_status, head);
	}
	execve(command->cmdpath, command->cmd_arr, g_utils.environment);
	execute_error(command->cmd_arr[0], head);
}

static void	subshell(t_exec *exec, t_node *command, t_node *par_temp,
	t_node **head)
{
	if (command->previous && command->previous->type == PIPE)
		ft_dup2(exec->tmp_fd, STDIN_FILENO, head, EXIT);
	if (par_temp->next && par_temp->next->type == PIPE)
		ft_dup2(exec->pipe[1], STDOUT_FILENO, head, EXIT);
	ft_close(exec->pipe[0], "subshell", head, EXIT);
	ft_close(exec->pipe[1], "subshell", head, EXIT);
	ft_close(exec->tmp_fd, "subshell", head, EXIT);
	command = command->next;
	command->previous = NULL;
	par_temp->previous->next = NULL;
	executor(command, exec->process_lvl + 1, head);
	ft_exit(g_utils.exit_status, head);
}

static void	builtin(t_exec *exec, t_node *command, t_node **head)
{
	int	tmp_in;
	int	tmp_out;

	if (parse_command(command, head) == ERROR)
	{
		g_utils.exit_status = EXIT_FAILURE;
		return ;
	}
	tmp_in = ft_dup(STDIN_FILENO, "builtin", head, NO_EXIT);
	tmp_out = ft_dup(STDOUT_FILENO, "builtin", head, NO_EXIT);
	if (command->in == HERE_DOC)
		retrieve_here_doc(command, head);
	else if (command->in != STDIN_FILENO)
		ft_dup2(command->in, STDIN_FILENO, head, NO_EXIT);
	if (command->out != STDOUT_FILENO)
		ft_dup2(command->out, STDOUT_FILENO, head, NO_EXIT);
	g_utils.exit_status = execute_builtin (command, exec->process_lvl, head);
	ft_dup2(tmp_in, STDIN_FILENO, head, NO_EXIT);
	ft_dup2(tmp_out, STDOUT_FILENO, head, NO_EXIT);
	ft_close(tmp_in, "builtin", head, NO_EXIT);
	ft_close(tmp_out, "builtin", head, NO_EXIT);
}

static void	parent(t_exec *exec, t_node **head)
{
	ft_close(exec->pipe[1], "parent", head, NO_EXIT);
	ft_close(exec->tmp_fd, "parent", head, NO_EXIT);
	exec->tmp_fd = ft_dup(exec->pipe[0], "parent", head, NO_EXIT);
	ft_close(exec->pipe[0], "parent", head, NO_EXIT);
}

void	execute_command(t_exec *exec, t_node **command, t_node **head)
{
	t_node	*par_temp;

	par_temp = *command;
	if ((*command)->type == LPAREN)
		par_temp = skip_paren_content(*command, 0);
	ft_pipe(exec->pipe, "execute_command", head, NO_EXIT);
	if (is_pipeline(*command) || par_temp != *command
		|| !check_builtin((*command)->args))
		exec->pid = ft_fork("execute_command", head, NO_EXIT);
	else
		builtin(exec, *command, head);
	if (!exec->pid && par_temp != *command)
		subshell(exec, *command, par_temp, head);
	else if (!exec->pid)
		child(exec, *command, head);
	if (exec->pid)
		parent(exec, head);
	*command = par_temp;
}
