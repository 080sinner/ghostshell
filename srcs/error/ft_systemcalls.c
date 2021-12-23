/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_systemcalls.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/22 19:21:54 by fbindere          #+#    #+#             */
/*   Updated: 2021/12/23 22:52:01 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_dup2(int fd1, int fd2, t_node **head, int exit_flag)
{
	if (dup2(fd1, fd2) == ERROR)
	{
		ft_putstr_fd("how scary: dup2 failed", 2);
		if (exit_flag == 1)
			ft_exit(EXIT_FAILURE, head);
	}
}

int	ft_dup(int fd, char *function, t_node **head, int exit_flag)
{
	int fd_return;

	fd_return = dup(fd);
	
	if (fd_return == ERROR)
	{
		ft_putstr_fd("how scary: dup failed in function: ", 2);
		ft_putstr_fd(function, 2);
		if (exit_flag == 1)
			ft_exit(EXIT_FAILURE, head);
	}
	return (fd_return);
}

int		ft_fork(char *function, t_node **head, int exit_flag)
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

void	ft_close(int fd, char *function, t_node **head, int exit_flag)
{
	if (close(fd) == ERROR)
	{
		ft_putstr_fd("how scary: close failed in function: ", 2);
		ft_putstr_fd(function, 2);
		if (exit_flag == 1)
			ft_exit(EXIT_FAILURE, head);
	}
}

int	ft_open(char *file, int type)
{
	int	fd;

	fd = ERROR;
	if (type == LESS)
		fd = open(file, O_RDONLY, 0644);
	else if (type == GREAT)
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (type == GREATGREAT)
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == ERROR)
	{
		if (errno == NORIGHTS)
		{
			ft_putstr_fd("how scary: permission denied: ", 2);
			ft_putstr_fd(file, 2);
		}
		if (errno == NOFILE)
		{
			ft_putstr_fd("how scary: no such file or directory: ", 2);
			ft_putstr_fd(file, 2);
		}
		ft_putstr_fd("\n", 2);
		return (ERROR);
	}
	return(fd);
}

void	ft_exit(int status, t_node **head)
{
	free_nodes(head);
	if (status == EXIT_SUCCESS)
		exit(EXIT_SUCCESS);
	else
		exit(EXIT_FAILURE);
}