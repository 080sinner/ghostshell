/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 17:37:26 by fbindere          #+#    #+#             */
/*   Updated: 2022/01/05 17:44:34 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_exec(t_exec *exec, t_node **head)
{
	exec->exit_status = 0;
	exec->pid = 1;
	exec->tmp_fd = ft_dup(STDIN_FILENO, "init_exec", head, NO_EXIT);
}

int	is_pipeline(t_node *command)
{	
	if (command->next && command->next->type == PIPE)
		return (1);
	if (command->previous && command->previous->type == PIPE)
		return (1);
	return (0);
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

void	retrieve_here_doc(t_node *command, t_node **head)
{
	t_tok	*token;
	int		here_pipe[2];

	ft_pipe(here_pipe, "retrieve_here_doc", head, NO_EXIT);
	token = command->here_doc;
	while (token)
	{
		if (token->data)
			ft_putendl_fd(token->data, here_pipe[1]);
		token = token->next;
	}
	ft_dup2(here_pipe[0], STDIN_FILENO, head, NO_EXIT);
	ft_close(here_pipe[1], "retrieve_here_doc", head, NO_EXIT);
	ft_close(here_pipe[0], "retrieve_here_doc", head, NO_EXIT);
}
