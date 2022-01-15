/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 23:02:11 by fbindere          #+#    #+#             */
/*   Updated: 2022/01/15 22:19:22 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	unset_error(char *varname)
{
	ft_putstr_fd("Haunting mistake: unset: '", 2);
	ft_putstr_fd(varname, 2);
	ft_putendl_fd("' not a valid identifier", 2);
	return (1);
}

static void	unset_variable(char **tmp_env, char *variable, t_node **head)
{
	int	var_index;
	int	i;

	var_index = search_envvar(variable, g_utils.environment);
	ft_copy_env(g_utils.environment, var_index, head);
	i = -1;
	while (tmp_env[++i])
		ft_free((void *)&tmp_env[i], ft_strlen(tmp_env[i]));
	free(tmp_env);
}

int	unset(t_node *command, t_node **head)
{
	char	**tmp_env;
	int		i;
	int		count;
	int		ret;

	ret = 0;
	count = 0;
	while (command->cmd_arr[++count])
	{
		tmp_env = g_utils.environment;
		i = -1;
		while (++i)
			tmp_env[i] = g_utils.environment[i];
		if (check_valid_var_name(command->cmd_arr[count]))
			ret = unset_error(command->cmd_arr[count]);
		else
			unset_variable(tmp_env, command->cmd_arr[count], head);
	}
	return (ret);
}
