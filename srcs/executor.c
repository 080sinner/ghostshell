/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 17:44:02 by fbindere          #+#    #+#             */
/*   Updated: 2021/12/14 17:56:17 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


// 1. expand command
// 2. read input/output for command and remove red op.
// 3. aus liste char** createn,
// 3. 

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


void	set_input (t_node *command)
{
	t_tok *current;

	current = command->args;
	command->in = PIPEIN;	
	while (current)
	{
		if (current->type == LESS)
		{
			command->in = open(current->next->data, O_RDONLY, 0644);
			if (command->in == ERROR)
				exec_error (current->next->data);
		}
		else if (current->type == LESSLESS)
			command->in = HERE_DOC;
		else if (!command->previous || command->previous->type == OR 
			|| command->previous->type == AND)
			command->in = 0;
	}
}

void	exec_error(char *file)
{
	printf("error %s\n", file);
}

void	set_output (t_node *command)
{
	t_tok *current;

	current = command->args;
	command->out = PIPEOUT;	
	while (current)
	{
		if (current->type == GREAT)
		{
			command->out = open(current->next->data, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (command->out == ERROR)
				exec_error(current->next->data);
		}
		else if (current->type == GREATGREAT)
		{
			command->out = open(current->next->data, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (command->out == ERROR)
				exec_error(current->next->data);
		}	
		else if (!command->next || command->next->type == OR 
			|| command->next->type == AND)
			command->out = 1;
	}
}

void	child(t_node command, int *pipe1, int *pipe2)
{
	extern char	**environ;

	set_input(input, i, pipe1, pipe2);
	set_output(input, i, pipe1, pipe2);
	ft_getpath(input);
	execve(input->cmdpath, input->args, environ);
	exit(EXIT_FAILURE);
}

pid_t	execute_command(t_node *command, int pipe1[2], int pipe2[2])
{
	pid_t		pid;
	static int	i;

	// if (current->type == LPAREN)
		// 	current = executor(&current->next, skip_paren_content(current), level + 1);
	if (i % 2 == 0)
		ft_pipe(pipe1);
	else
		ft_pipe(pipe2);
	pid = ft_fork();
	if (pid == 0)
		child(i, pipe1, pipe2);
	else
		parent(i, pipe1, pipe2);
	i++;
}

t_node	*executor(t_node **head, t_node *end_of_loop, int pipe1[2], int pipe2[2])
{
	t_node *current;
	pid_t	pid;

	current = *head;
	while (current != end_of_loop)
	{
		if (current->type == COMMAND || current->type == LPAREN)
		{
			execute_command(current, pipe1, pipe2);
			// if (current->type == LPAREN)
			// 	current = executor(&current->next, skip_paren_content(current), level + 1);
		}
		if (!current || (current && (current->type == OR || current->type == AND)))
		{
			// wait()
			if (!current)
				break ;
			// if(check_exit_status())
			// 	skip_pipe_line;
		}
		current = current->next;
	}
	return (current);
}
