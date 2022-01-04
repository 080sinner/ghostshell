/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/18 17:09:14 by fbindere          #+#    #+#             */
/*   Updated: 2022/01/04 19:11:45 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void init_exec(t_exec *exec, t_node **head)
{
	exec->exit_status = 0;
	exec->pid = 1;
	exec->tmp_fd = ft_dup(STDIN_FILENO, "init_exec", head, NO_EXIT);
}

void parent(t_exec *exec, t_node **head)
{
	ft_close(exec->pipe[1], "parent", head, NO_EXIT);
	ft_close(exec->tmp_fd, "parent", head, NO_EXIT);
	exec->tmp_fd = ft_dup(exec->pipe[0], "parent", head, NO_EXIT);
	ft_close(exec->pipe[0], "parent", head, NO_EXIT);
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

int	set_output(t_node *command, t_tok *arg)
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

int	set_input(t_node *command, t_tok *arg, t_node **head)
{

	if (arg->type == LESS)
	{
		if (arg->next && arg->next->data)
		{
			if (command->in != PIPEIN && command->in != STDIN_FILENO && command->in != HERE_DOC)
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

int	set_redirections(t_node *command, t_node **head)
{
	t_tok *current;

	if (!command || !command->args)
		return (ERROR);
	current = command->args;
	command->in = PIPEIN;
	command->out = PIPEOUT;
	if (!command->next || command->next->type == OR || command->next->type == AND)
		command->out = 1;
	if (!command->previous || command->previous->type == OR || command->previous->type == AND)
		command->in = STDIN_FILENO;
	while(current)
	{
		if (current->type == LESS || current->type == LESSLESS)
		{
			if(set_input(command, current, head) == ERROR)
				return (ERROR);
		}
		else if(current->type == GREAT || current->type == GREATGREAT)
		{
			if (set_output(command, current) == ERROR)
				return (ERROR);
		}
		current = current->next;
	}
	return (1);
}


int	create_array(t_node *command)
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
		return(ERROR);
	current = command->args;
	args_count = 0;
	while(current)
	{
		if (current->data)
			command->cmd_arr[args_count++] = current->data;
		current = current->next;
	}
	return (1);
}

int	parse_command(t_node *current, t_node **head)
{
	if (!current)
		return (ERROR);
	expander(current, head);
	if (set_redirections(current, head) == ERROR)
		return (ERROR);
	if (create_array(current) == ERROR)
		return(ERROR);
	return (1);
}

void	retrieve_here_doc(t_node *command, t_node **head)
{
	t_tok	*token;
	int		here_pipe[2];
	
	ft_pipe(here_pipe, "retrieve_here_doc", head, NO_EXIT);
	token = command->here_doc;
	while(token)
	{
		if (token->data)
			ft_putendl_fd(token->data, here_pipe[1]);
		token = token->next;
	}
	ft_dup2(here_pipe[0], STDIN_FILENO, head, NO_EXIT);
	ft_close(here_pipe[1], "retrieve_here_doc", head, NO_EXIT);
	ft_close(here_pipe[0], "retrieve_here_doc", head, NO_EXIT);
}

void child(t_exec *exec, t_node *command, t_node **head)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (parse_command(command, head) == ERROR || get_cmd_path(command) == ERROR)
		ft_exit(EXIT_FAILURE, head);
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
		if(!execute_builtin (command, head))
			ft_exit (EXIT_SUCCESS, head);
		ft_exit (EXIT_FAILURE, head);
	}
	execve(command->cmdpath, command->cmd_arr, g_utils.environment);
	ft_exit(EXIT_FAILURE, head);
}

t_node	*skip_paren_content(t_node *current, int first_call)
{
	static int	parencount;

	if (first_call == 0)
		parencount = 0;
	if (!current)
		return (NULL);
	if (current->type == LPAREN)
		parencount++;
	if (current->type == RPAREN)
		parencount--;
	if (parencount == 0)
		return (current);
	return (skip_paren_content(current->next, ++first_call));
}


void subshell (t_exec *exec, t_node *command, t_node *par_temp, t_node **head)
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
	executor(command, head);
	ft_exit(EXIT_SUCCESS, head);
}

int	is_pipeline(t_node *command)
{	
	if (command->next && command->next->type == PIPE)
		return (1);
	if (command->previous && command->previous->type == PIPE)
		return (1);
	return (0);
}

void builtin (t_node *command, t_node **head)
{
	int tmp_in;
	int tmp_out;
	
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
	g_utils.exit_status = execute_builtin (command, head);
	ft_dup2(tmp_in, STDIN_FILENO, head, NO_EXIT);
	ft_dup2(tmp_out, STDOUT_FILENO, head, NO_EXIT);
	ft_close(tmp_in, "builtin", head, NO_EXIT);
	ft_close(tmp_out, "builtin", head, NO_EXIT);
}

void execute_command (t_exec *exec, t_node **command, t_node **head)
{
	t_node *par_temp;
	
	par_temp = *command;
	if ((*command)->type == LPAREN)
		par_temp = skip_paren_content(*command, 0);
	ft_pipe(exec->pipe, "execute_command", head, NO_EXIT);
	if (is_pipeline(*command) || par_temp != *command || !check_builtin((*command)->args))
		exec->pid = ft_fork("execute_command", head, NO_EXIT);
	else
		builtin(*command, head);
	if (!exec->pid && par_temp != *command)
		subshell(exec, *command, par_temp, head);
	else if (!exec->pid)
		child(exec, *command, head);
	if (exec->pid)
		parent(exec, head);
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

	init_exec(&exec, head);
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
		if (current && current->type == OR && g_utils.exit_status == 0)
			current = skip_pipeline(current);
		else if (current && current->type == AND && g_utils.exit_status != 0)
			current = skip_pipeline(current);
		if (!current)
			break;
		current = current->next;
	}
}
