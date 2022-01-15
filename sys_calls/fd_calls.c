/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_calls.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 19:29:04 by fbindere          #+#    #+#             */
/*   Updated: 2022/01/15 22:18:19 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_dup2(int fd1, int fd2, t_node **head, int exit_flag)
{
	if (dup2(fd1, fd2) == ERROR)
	{
		ft_putstr_fd("Better run! Dup2 failed", 2);
		if (exit_flag == 1)
			ft_exit(g_utils.exit_status, head);
	}
}

int	ft_dup(int fd, char *function, t_node **head, int exit_flag)
{
	int	fd_return;

	fd_return = dup(fd);
	if (fd_return == ERROR)
	{
		ft_putstr_fd("Better run! Dup failed in function: ", 2);
		ft_putstr_fd(function, 2);
		if (exit_flag == 1)
			ft_exit(g_utils.exit_status, head);
	}
	return (fd_return);
}

void	ft_close(int fd, char *function, t_node **head, int exit_flag)
{
	if (close(fd) == ERROR)
	{
		ft_putstr_fd("Better run! Close failed in function: ", 2);
		ft_putstr_fd(function, 2);
		if (exit_flag == 1)
			ft_exit(g_utils.exit_status, head);
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
			ft_putstr_fd("Better run! Permission denied: ", 2);
			ft_putstr_fd(file, 2);
		}
		if (errno == NOFILE)
		{
			ft_putstr_fd("Better run! No such file or directory: ", 2);
			ft_putstr_fd(file, 2);
		}
		ft_putstr_fd("\n", 2);
		return (ERROR);
	}
	return (fd);
}
