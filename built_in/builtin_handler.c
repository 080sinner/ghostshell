/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 21:48:51 by fbindere          #+#    #+#             */
/*   Updated: 2022/01/05 23:09:38 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	execute_builtin(t_node *command, t_node **head)
{
	if (command->cmd_arr && command->cmd_arr[0])
	{
		if (ft_strcmp(command->cmd_arr[0], "cd"))
			return (change_dir(command->cmd_arr[1]));
		else if (ft_strcmp(command->cmd_arr[0], "echo"))
			return (echo(command->cmd_arr));
		else if (ft_strcmp(command->cmd_arr[0], "pwd"))
			return (print_pwd());
		else if (ft_strcmp(command->cmd_arr[0], "export"))
			return (export(command, head));
		else if (ft_strcmp(command->cmd_arr[0], "unset"))
			return (unset(command, head));
		else if (ft_strcmp(command->cmd_arr[0], "env"))
			return (print_env(0));
		else if (ft_strcmp(command->cmd_arr[0], "exit"))
			exit_builtin(head);
	}
	return (ERROR);
}

int	check_builtin(t_tok *command)
{
	char	*builtins[7];
	int		i;

	if (command == NULL)
		return (0);
	while (command->type != COMMAND)
		command = command->next;
	if (command->previous && command->previous->type != COMMAND)
		command = command->next;
	ft_striteri(command->data, ft_tolower);
	builtins[0] = "cd";
	builtins[1] = "echo";
	builtins[2] = "pwd";
	builtins[3] = "export";
	builtins[4] = "env";
	builtins[5] = "unset";
	builtins[6] = "exit";
	i = 0;
	while (i <= 6)
	{
		if (ft_strcmp(command->data, builtins[i]))
			return (1);
		i++;
	}
	return (0);
}
