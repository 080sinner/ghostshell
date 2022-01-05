/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_arr.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 17:29:24 by fbindere          #+#    #+#             */
/*   Updated: 2022/01/05 17:29:56 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	create_array(t_node *command)
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
		current = current->next;
	}
	command->cmd_arr = ft_calloc(args_count + 1, sizeof(char *));
	if (!command->cmd_arr)
		return (ERROR);
	current = command->args;
	args_count = 0;
	while (current)
	{
		if (current->data)
			command->cmd_arr[args_count++] = current->data;
		current = current->next;
	}
	return (1);
}
