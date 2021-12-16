/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 17:44:02 by fbindere          #+#    #+#             */
/*   Updated: 2021/12/16 23:44:06 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


// 1. expand command
// 2. read input/output for command and remove red op.
// 3. aus liste char** createn,
// 3. 


void	exec_error(char *file)
{
	printf("error %s\n", file);
}

t_node	*skip_paren_content(t_node *current)
{
	static int parencount;

	if (!current)
		return (NULL);
	if (parencount == 0)
		parencount = 1;
	if (current->type == LPAREN)
		parencount++;
	if (current->type == RPAREN)
		parencount--;
	if (parencount == 0)
		return (current);
	return (skip_paren_content(current->next));
}

void	set_input(t_node **command)
{
	t_tok *current;

	if (!*command || !(*command)->args)
		return ;
	current = (*command)->args;
	(*command)->in = PIPEIN;	
	if (!(*command)->previous || (*command)->previous->type == OR 
			|| (*command)->previous->type == AND)
			(*command)->in = 0;
	while (current)
	{
		if (current->type == LESS)
		{
			if (current->next && current->next->data)
				(*command)->in = open(current->next->data, O_RDONLY, 0644);
			if ((*command)->in == ERROR)
				exec_error(current->next->data);
			if (current && current->next && current->next->data)
				free(current->next->data);
			free(detach_tok(&(*command)->args, current->next));
			free(detach_tok(&(*command)->args, current));
		}
		else if (current->type == LESSLESS)
		{
			(*command)->in = HERE_DOC;
			if (current && current->next && current->next->data)
				free(current->next->data);
			free(detach_tok(&(*command)->args, current->next));
			free(detach_tok(&(*command)->args, current));
		}
		current = current->next;
	}
}

void	set_output(t_node **command)
{
	t_tok *current;

	if (!*command || !(*command)->args)
		return ;
	current = (*command)->args;
	(*command)->out = PIPEOUT;	
	while (current)
	{
		if (current->type == GREAT)
		{
			if (current->next && current->next->data)
				(*command)->out = open(current->next->data, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if ((*command)->out == ERROR)
				exec_error(current->next->data);
			if (current && current->next && current->next->data)
				free(current->next->data);
			free(detach_tok(&(*command)->args, current->next));
			free(detach_tok(&(*command)->args, current));
		}
		else if (current->type == GREATGREAT)
		{
			if (current->next && current->next->data)
				(*command)->out = open(current->next->data, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if ((*command)->out == ERROR)
				exec_error(current->next->data);
			if (current && current->next && current->next->data)
				free(current->next->data);
			free(detach_tok(&(*command)->args, current->next));
			free(detach_tok(&(*command)->args, current));
		}	
		current = current->next;
	}
	if (!(*command)->next || (*command)->next->type == OR 
			|| (*command)->next->type == AND)
			(*command)->out = 1;
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

void	parse_command(t_node **current, t_node **head)
{
	if (!*current)
		return ;
	expander(*current, head);
	set_input(current);
	set_output(current);
	get_cmd_path(*current);
	create_array(*current);
	// if ((*current)->args)
	// 	printf("command : %s | input : %d | output : %d\n", (*current)->args->data, (*current)->in, (*current)->out);
}



void	detach_parentheses(t_node **head, t_node **current)
{
	t_node	*tmp;
	t_node	*tmp2;

	tmp2 = NULL;
	tmp = detach_node(head, *current);
	*current = (*current)->next;
	free(tmp);
	tmp = skip_paren_content(*current);
	if (tmp)
		tmp2 = tmp->next;
	free(detach_node(head, tmp));
}

void parent (t_exec *exec)
{
	if (exec->cmd_count % 2 == EVEN)
	{
		close(exec->pipe1[1]);
		if (exec->cmd_count != 0)
			close(exec->pipe2[0]);
	}
	else if(exec->cmd_count %2 == ODD)
	{
		close(exec->pipe1[0]);		
		close(exec->pipe2[1]);
	}
}

void	child (t_exec *exec, t_node *command)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (exec->cmd_count % 2 == EVEN)
	{
		if (command->in == PIPEIN)
			dup2(exec->pipe2[0], STDIN_FILENO);
		else
			dup2(command->in, STDIN_FILENO);
		if (command->out == PIPEOUT)
			dup2(exec->pipe1[1], STDOUT_FILENO);
		else
			dup2(command->out, STDOUT_FILENO);
		close(exec->pipe1[1]);
		close(exec->pipe1[0]);
		if (exec->cmd_count != 0)
			close(exec->pipe2[0]);
	}
	else if (exec->cmd_count % 2 == ODD)
	{
		if (command->in == PIPEIN)
			dup2(exec->pipe1[0], STDIN_FILENO);
		else
			dup2(command->in, STDIN_FILENO);
		if (command->out == PIPEOUT)
			dup2(exec->pipe2[1], STDOUT_FILENO);
		else
			dup2(command->out, STDOUT_FILENO);
		close(exec->pipe1[0]);	
		close(exec->pipe2[1]);
		close(exec->pipe2[0]);		
	}
	execve(command->cmdpath, command->cmd_arr, g_utils.environment);
	exit (EXIT_FAILURE);
}

void	execute_command (t_exec *exec, t_node **command, t_node **head)
{
	parse_command(command, head);
	if (exec->cmd_count % 2 == EVEN)
		pipe(exec->pipe1);
	else
		pipe(exec->pipe2);
	exec->pid = fork();
	if (exec->pid == 0)
		child(exec, *command);
	else if (exec->pid != 0)
		parent(exec);
	exec->cmd_count++;
}

t_node	*executor(t_node *current, t_node *end_of_loop, t_node **head)
{
	int	es;
	int	status;
	t_exec exec;

	init_exec(&exec);
	if (!current)
		current = *head;
	status = 0;
	while (!current || current != end_of_loop)
	{
		if (current && (current->type == COMMAND || current->type == LPAREN))
		{
			if (current->type == LPAREN)
				detach_parentheses(head, &current);
			execute_command(&exec, &current, head);
			//*current = executor(*current, tmp2, , head);
		}
		if (!current || (current && (current->type == OR || current->type == AND)))
		{
			if (exec.cmd_count % 2 == ODD)
				close(exec.pipe1[0]);
			else if(exec.cmd_count %2 == EVEN)
				close(exec.pipe2[0]);
			while (1)
			{
				es = waitpid(-1, &status, 0);
				if (exec.pid == es)
				{
					if (WIFEXITED(status))
						g_utils.exit_status = WEXITSTATUS(status);
					if (WIFSIGNALED(status))
					{
						if (WTERMSIG(status) == SIGQUIT)
							printf("Quit: %d\n", SIGQUIT);
						else if (WTERMSIG(status) == SIGINT)
							write(1, "\n", 1);
						g_utils.exit_status = 127 + WTERMSIG(status);
					}
				}
				if (es == -1)
					break ;
			}
			if (!current)
				break ;
			// if (current->type == OR && g_utils.exit_status == EXIT_SUCCESS)
			// 	skip_pipeline;
			// if (current->type == AND && g_utils.exit_status != EXIT_SUCCESS)
			// 	skip_pipeline;
		}
		current = current->next;
	}
	return (current);
}





