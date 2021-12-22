/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/18 17:09:14 by fbindere          #+#    #+#             */
/*   Updated: 2021/12/22 19:20:29 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void init_exec(t_exec *exec)
{
	exec->exit_status = 0;
	exec->pid = 1;
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

void	free_redir_op(t_tok **head, t_tok *node)
{
	if (node)
	{
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
}

void	set_output(t_node *command)
{
	t_tok *current;

	if (!command || !command->args)
		return ;
	current = command->args;
	command->out = PIPEOUT;	
	if (!command->next || command->next->type == OR 
		|| command->next->type == AND)
			command->out = 1;
	while (current)
	{
		if (current->type == GREAT)
		{
			if (current->next && current->next->data)
				command->out = open(current->next->data, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (command->out == ERROR)
				exec_error(current->next->data);
			free_redir_op(&command->args, current);
		}
		else if (current->type == GREATGREAT)
		{
			if (current->next && current->next->data)
				command->out = open(current->next->data, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (command->out == ERROR)
				exec_error(current->next->data);
			free_redir_op(&command->args, current);
		}
		current = current->next;
	}
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
			free_redir_op(&command->args, current);
		}
		else if (current->type == LESSLESS)
		{
			command->in = HERE_DOC;
			free_redir_op(&command->args, current);
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

void	retrieve_here_doc(t_node *command)
{
	t_tok	*token;
	int		here_pipe[2];
	
	pipe(here_pipe);
	token = command->here_doc;
	while(token)
	{
		if (token->data)
			ft_putendl_fd(token->data, here_pipe[1]);
		token = token->next;
	}
	dup2(here_pipe[0], STDIN_FILENO);
	close(here_pipe[1]);
	close(here_pipe[0]);
}

void child(t_exec *exec, t_node *command)
{
	if (command->in == PIPEIN)
		dup2(exec->tmp_fd, STDIN_FILENO);
	else if (command->in == HERE_DOC)
		retrieve_here_doc(command);
	else
		dup2(command->in, STDIN_FILENO);
	if (command->out == PIPEOUT)
		dup2(exec->pipe[1], STDOUT_FILENO);
	else
		dup2(command->out, STDOUT_FILENO);
	close(exec->pipe[0]);
	close(exec->pipe[1]);
	close(exec->tmp_fd);
	if (check_builtin(command))
	{
		if(!execute_builtin (command))
			exit (EXIT_SUCCESS);
		exit (EXIT_FAILURE);
	}
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

void subshell (t_exec *exec, t_node *command, t_node *par_temp, t_node **head)
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

int	is_pipeline(t_node *command)
{	
	if (command->next && command->next->type == PIPE)
		return (1);
	if (command->previous && command->previous->type == PIPE)
		return (1);
	return (0);
}

void builtin (t_node *command)
{
	int tmp_in;
	int tmp_out;
	
	tmp_in = dup(STDIN_FILENO);
	tmp_out = dup(STDOUT_FILENO);
	if (command->in == HERE_DOC)
		retrieve_here_doc(command);
	else if (command->in != STDIN_FILENO)
		dup2(command->in, STDIN_FILENO);
	if (command->out != STDOUT_FILENO)
		dup2(command->out, STDOUT_FILENO);
	g_utils.exit_status = execute_builtin (command);
	dup2(tmp_in, STDIN_FILENO);
	dup2(tmp_out, STDOUT_FILENO);
	close(tmp_in);
	close(tmp_out);
}

void execute_command (t_exec *exec, t_node **command, t_node **head)
{
	t_node *par_temp;
	
	par_temp = *command;
	parse_command(*command, head);
	if ((*command)->type == LPAREN)
		par_temp = skip_paren_content(*command);
	pipe(exec->pipe);
	if (!check_builtin(*command) || is_pipeline(*command))
		exec->pid = fork();
	else
		builtin(*command);
	if (!exec->pid && par_temp != *command)
		subshell(exec, *command, par_temp, head);
	else if (!exec->pid)
		child(exec, *command);
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

t_node *skip_pipeline(t_node *command)
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
		return(skip_pipeline(command));
}


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
		}
		if (current && current->type == OR && g_utils.exit_status == 0)
			current = skip_pipeline(current);
		else if (current && current->type == AND && g_utils.exit_status != 0)
			current = skip_pipeline(current);
		if (!current)
			break;
		current = current->next;
	}
}
