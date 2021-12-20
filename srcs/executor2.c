/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/18 17:09:14 by fbindere          #+#    #+#             */
/*   Updated: 2021/12/20 20:48:35 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void init_exec(t_exec *exec)
{
	exec->exit_status = 0;
	exec->pid = 0;
	exec->tmp_fd = dup(STDIN_FILENO);
}

void	exec_error(char *file)
{
	printf("error %s\n", file);
}

void parent(t_exec *exec)
{
	close(exec->pipe[1]);
	close(exec->tmp_fd);
	exec->tmp_fd = dup(exec->pipe[0]);
	close(exec->pipe[0]);
}

void	set_output(t_node *command)
{
	t_tok *current;

	if (!command || !command->args)
		return ;
	current = command->args;
	command->out = PIPEOUT;	
	while (current)
	{
		if (current->type == GREAT)
		{
			if (current->next && current->next->data)
				command->out = open(current->next->data, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (command->out == ERROR)
				exec_error(current->next->data);
			if (current && current->next && current->next->data)
				free(current->next->data);
			free(detach_tok(&command->args, current->next));
			free(detach_tok(&command->args, current));
		}
		else if (current->type == GREATGREAT)
		{
			if (current->next && current->next->data)
				command->out = open(current->next->data, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (command->out == ERROR)
				exec_error(current->next->data);
			if (current && current->next && current->next->data)
				free(current->next->data);
			free(detach_tok(&command->args, current->next));
			free(detach_tok(&command->args, current));
		}	
		current = current->next;
	}
	if (!command->next || command->next->type == OR 
		|| command->next->type == AND)
			command->out = 1;
}

void	set_input(t_node *command)
{
	t_tok *current;

	if (!command || !command->args)
		return ;
	current = command->args;
	command->in = PIPEIN;
	if (!command->previous || command->previous->type == OR 
			|| command->previous->type == AND)
			command->in = 0;
	while (current)
	{
		if (current->type == LESS)
		{
			if (current->next && current->next->data)
				command->in = open(current->next->data, O_RDONLY, 0644);
			if (command->in == ERROR)
				exec_error(current->next->data);
			if (current && current->next && current->next->data)
				free(current->next->data);
			free(detach_tok(&command->args, current->next));
			free(detach_tok(&command->args, current));
		}
		else if (current->type == LESSLESS)
		{
			command->in = HERE_DOC;
			if (current && current->next && current->next->data)
				free(current->next->data);
			free(detach_tok(&command->args, current->next));
			free(detach_tok(&command->args, current));
		}
		current = current->next;
	}
}

void	create_array(t_node *command)
{
	t_tok	*current;
	int		args_count;
	
	current = NULL;
	if (command->args)
		current = command->args;
	args_count = 0;
	while (current)
	{
		args_count++;
		current =  current->next;
	}
	command->cmd_arr = ft_calloc(args_count + 1, sizeof(char *));
	if (!command->cmd_arr)
		exit(EXIT_FAILURE);
	current = command->args;
	args_count = 0;
	while(current)
	{
		if (current->data)
			command->cmd_arr[args_count++] = current->data;
		current = current->next;
	}
}

void	parse_command(t_node *current, t_node **head)
{
	if (!current)
		return ;
	expander(current, head);
	set_input(current);
	set_output(current);
	get_cmd_path(current);
	create_array(current);
}

void child (t_exec *exec, t_node *command, t_node **head)
{
	parse_command(command, head);
	if (command->in == PIPEIN)
		dup2(exec->tmp_fd, STDIN_FILENO);
	else
		dup2(command->in, STDIN_FILENO);
	if (command->out == PIPEOUT)
		dup2(exec->pipe[1], STDOUT_FILENO);
	else
		dup2(command->out, STDOUT_FILENO);
	close(exec->pipe[0]);
	close(exec->pipe[1]);
	close(exec->tmp_fd);
	execve(command->cmdpath, command->cmd_arr, g_utils.environment);
	exit(EXIT_FAILURE);
}

t_node	*skip_paren_content(t_node *current)
{
	static int parencount;

	if (!current)
		return (NULL);
	if (current->type == LPAREN)
		parencount++;
	if (current->type == RPAREN)
		parencount--;
	if (parencount == 0)
		return (current);
	return (skip_paren_content(current->next));
}

void child_paren (t_exec *exec, t_node *command, t_node *par_temp, t_node **head)
{
	if (command->previous && command->previous->type == PIPE)
		dup2(exec->tmp_fd, STDIN_FILENO);
	if (par_temp->next && par_temp->next->type == PIPE)
		dup2(exec->pipe[1], STDOUT_FILENO);
	close(exec->pipe[0]);
	close(exec->pipe[1]);
	close(exec->tmp_fd);
	command = command->next;
	command->previous = NULL;
	par_temp->previous->next = NULL;
	executor(command, head);
	exit(EXIT_SUCCESS);
}

void execute_command (t_exec *exec, t_node **command, t_node **head)
{
	t_node *par_temp;
	
	par_temp = *command;
	if ((*command)->type == LPAREN)
		par_temp = skip_paren_content(*command);
	pipe(exec->pipe);
	exec->pid = fork();
	if (!exec->pid && par_temp != *command)
		child_paren(exec, *command, par_temp, head);
	if (!exec->pid)
		child(exec, *command, head);
	if (exec->pid)
		parent(exec);
	*command = par_temp;
}

void	exit_status(t_exec *exec)
{
	int exit_status;
	int status;
	
	while (1)
	{
		exit_status = waitpid(-1, &status, 0);
		if (exec->pid == exit_status)
		{
			if (WIFEXITED(status))
				g_utils.exit_status = WEXITSTATUS(status);
			if (WIFSIGNALED(status))
			{
				if (WTERMSIG(status) == SIGQUIT)
				{
					ft_putstr_fd("Quit: ", 2);
					ft_putnbr_fd(SIGQUIT, 2);
					ft_putendl_fd("", 2);
				}
				else if (WTERMSIG(status) == SIGINT)
					ft_putendl_fd("", 2);
				g_utils.exit_status = 127 + WTERMSIG(status);
			}
		}
	if (exit_status == -1)
		break ;
	}
}

// t_node *skip_pipe_line()
// {
// 	t_node current;

// 	return (current);
// }

void executor (t_node *current, t_node **head)
{
	t_exec	exec;

	init_exec(&exec);
	while (1)
	{
		if (current && (current->type == COMMAND || current->type == LPAREN))
			execute_command(&exec, &current, head);
		if (!current || current->type == OR || current->type == AND)
		{
			close(exec.tmp_fd);
			exec.tmp_fd = dup(STDIN_FILENO);
			exit_status(&exec);
			// if (current->type == OR && g_utils.exit_status == EXIT_SUCCESS)
			// 	skip_pipline;
			// if (current->type == AND && g_utils.exit_status == !EXIT_SUCCESS)
			// 	skip_pipeline;
		}
		if (!current)
			break;
		current = current->next;
	}
	return ;
}