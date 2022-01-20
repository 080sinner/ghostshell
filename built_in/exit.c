/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 21:52:43 by fbindere          #+#    #+#             */
/*   Updated: 2022/01/20 18:59:50 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_inputcheck(char *str, int *error, int *sign)
{
	int		i;
	int		signs;

	i = 0;
	signs = 0;
	if (str == NULL)
	{
		*error = ERROR;
		return ;
	}
	while (str[i] != '\0')
	{
		if (ft_strchr("0123456789+-", str[i]) == 0)
			break ;
		if (str[i] == '+' || str[i] == '-')
			signs++;
		if (str[i] == '-')
			*sign = -1;
		i++;
	}
	if (i > 11 || signs > 1 || str[i] != '\0'
		|| (signs == 1 && (str[0] != '+' && str[0] != '-')))
		*error = ERROR;
}

int	ft_my_atoi(char *str, int *error)
{
	int		value;
	int		sign;
	int		i;

	sign = 1;
	i = 0;
	value = 0;
	ft_inputcheck(str, error, &sign);
	if (*error == ERROR)
		return (0);
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] != '\0')
		value = value * 10 + str[i++] - '0';
	return (value * sign);
}

static int	check_exit_status(t_node *command, int error)
{
	int	integer;

	if (command->cmd_arr[1])
		integer = ft_my_atoi(command->cmd_arr[1], &error);
	if (command->cmd_arr[1] && error == ERROR)
	{
		ft_putendl_fd("Haunting mistake: exit: numeric argument required", 2);
		g_utils.exit_status = 255;
		return (1);
	}
	else if (command->cmd_arr[1] && command->cmd_arr[2])
	{
		ft_putendl_fd("Haunting mistake: exit: too many arguments", 2);
		g_utils.exit_status = 1;
		return (0);
	}
	else if (command->cmd_arr[1] && error != ERROR)
	{
		g_utils.exit_status = integer;
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
	if (command && check_exit_status(command, 0))
		ft_exit(g_utils.exit_status, head);
	else if (!command)
		ft_exit(g_utils.exit_status, head);
	return (g_utils.exit_status);
}
