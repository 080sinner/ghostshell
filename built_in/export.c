/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 23:48:52 by fbindere          #+#    #+#             */
/*   Updated: 2022/01/20 18:00:23 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	check_existing_variable(char **env, int *flag,
	int i, t_node *command)
{
	if (!env[VAR_CONTENT])
	{
		env[VAR_CONTENT] = ft_strdup("");
		env[VAR_NAME] = command->cmd_arr[i];
	}
	else
	{
		*flag = 1;
		*(ft_strchr(command->cmd_arr[i], '=')) = '\0';
		env[VAR_NAME] = command->cmd_arr[i];
	}
}

static void	export2(int *env_len, int *ret, char **envvar, int *env_flag)
{
	if (check_valid_var_name(envvar[VAR_NAME]))
	{
		ft_putstr_fd("Haunting mistake: export: ", 2);
		ft_putstr_fd(envvar[VAR_NAME], 2);
		ft_putendl_fd(" not a valid identifier", 2);
		ft_free((void *)&envvar[VAR_CONTENT], ft_strlen(envvar[1]));
		*ret = 1;
		envvar[VAR_NAME] = NULL;
	}
	if (*env_flag == 1 && search_envvar(envvar[VAR_NAME], g_utils.environment)
		== -1)
		g_utils.environment[*env_len] = ft_strjoin(envvar[0], envvar[1]);
	else if (*env_flag == 1 && envvar[VAR_NAME])
	{
		*env_len = search_envvar(envvar[VAR_NAME], g_utils.environment);
		free(g_utils.environment[*env_len]);
		g_utils.environment[*env_len] = ft_strjoin(envvar[0], envvar[1]);
	}
	else if (search_envvar(envvar[VAR_NAME], g_utils.environment) == -1)
		g_utils.environment[*env_len] = ft_strjoin(".", envvar[VAR_NAME]);
	ft_free((void *)&envvar[VAR_CONTENT], ft_strlen(envvar[1]));
}

int	export(t_node *command, t_node **head)
{
	int		env_len;
	char	*envvar[2];
	int		i;
	int		env_flag;
	int		ret;

	i = 0;
	ret = 0;
	if (ft_strcmp(command->cmd_arr[0], "export") && !command->cmd_arr[1])
		return (print_env(1));
	while (command->cmd_arr[++i])
	{
		env_flag = 0;
		env_len = create_new_env(g_utils.environment, head);
		envvar[VAR_CONTENT] = ft_strdup(ft_strchr(command->cmd_arr[i], '='));
		check_existing_variable(envvar, &env_flag, i, command);
		export2(&env_len, &ret, envvar, &env_flag);
	}
	return (ret);
}
