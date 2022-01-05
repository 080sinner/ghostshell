/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_calls.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/22 19:21:54 by fbindere          #+#    #+#             */
/*   Updated: 2022/01/06 00:09:05 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_fork(char *function, t_node **head, int exit_flag)
{
	pid_t	pid;

	pid = fork();
	if (pid == ERROR)
	{
		ft_putstr_fd("how scary: fork failed in function: ", 2);
		ft_putstr_fd(function, 2);
		if (exit_flag == 1)
			ft_exit(EXIT_FAILURE, head);
	}
	return (pid);
}

void	ft_pipe(int *fds, char *function, t_node **head, int exit_flag)
{
	if (pipe(fds) == ERROR)
	{
		ft_putstr_fd("how scary: pipe failed in function: ", 2);
		ft_putstr_fd(function, 2);
		if (exit_flag == 1)
			ft_exit(EXIT_FAILURE, head);
	}
}

void	ft_exit(int status, t_node **head)
{
	free_nodes(head);
	if (status == EXIT_SUCCESS)
		exit(EXIT_SUCCESS);
	else
		exit(EXIT_FAILURE);
}
