/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 21:52:43 by fbindere          #+#    #+#             */
/*   Updated: 2022/01/10 22:58:46 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	check_exit_status(t_node *command)
{
	if (command->cmd_arr[1] && command->cmd_arr[2])
	{
		ft_putendl_fd("how spooky: exit: too many arguments", 2);
		g_utils.exit_status = 1;
		return (0);
	}
	else if (command->cmd_arr[1] && !ft_isint(command->cmd_arr[1]))
	{
		ft_putendl_fd("how spooky: exit: numeric argument required", 2);
		g_utils.exit_status = 255;
		return (1);
	}
	else if (command->cmd_arr[1] && ft_isint(command->cmd_arr[1]))
	{
		g_utils.exit_status = ft_atoi(command->cmd_arr[1]);
		return (1);
	}
	return (1);
}

int	exit_builtin(t_node **head, t_node *command, int process_lvl)
{
	char	*username;

	if (process_lvl == 0)
	{
		ft_putstr_fd("Frighten no more ", 2);
		username = ft_strdup(ft_getenv("USER", g_utils.environment));
		if (username)
		{
			ft_putstr_fd(username, 2);
			free(username);
		}
		else
			ft_putstr_fd("visitor", 2);
		ft_putstr_fd("! You escaped the GHOSTSHELL!\n", 2);
	}
	if (check_exit_status(command))
		ft_exit(g_utils.exit_status, head);
	return (g_utils.exit_status);
}
