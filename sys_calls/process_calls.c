/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_calls.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/22 19:21:54 by fbindere          #+#    #+#             */
/*   Updated: 2022/01/15 22:18:40 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_fork(char *function, t_node **head, int exit_flag)
{
	pid_t	pid;

	pid = fork();
	if (pid == ERROR)
	{
		ft_putstr_fd("Better run! Fork failed in function: ", 2);
		ft_putstr_fd(function, 2);
		if (exit_flag == 1)
			ft_exit(g_utils.exit_status, head);
	}
	return (pid);
}

void	ft_pipe(int *fds, char *function, t_node **head, int exit_flag)
{
	if (pipe(fds) == ERROR)
	{
		ft_putstr_fd("Better run! Pipe failed in function: ", 2);
		ft_putstr_fd(function, 2);
		if (exit_flag == 1)
			ft_exit(g_utils.exit_status, head);
	}
}

void	ft_exit(int status, t_node **head)
{
	free_nodes(head);
	ft_free_strarray(g_utils.environment);
	exit(status);
}
