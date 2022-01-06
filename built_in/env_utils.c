/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 21:46:11 by fbindere          #+#    #+#             */
/*   Updated: 2022/01/05 23:33:08 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_valid_var_name(char *varname)
{
	int	i;

	if (!ft_isalpha(varname[0]) && varname[0] != '_')
		return (1);
	i = 1;
	while (varname[i])
	{
		if (!ft_isalnum(varname[i]) && varname[i] != '_')
			return (1);
		i++;
	}
	return (0);
}

int	create_new_env(char **env, t_node **head)
{
	char	**tmp_env;
	int		i;
	int		env_len;

	tmp_env = env;
	i = -1;
	while (env[++i])
		tmp_env[i] = env[i];
	env_len = i;
	ft_copy_env(g_utils.environment, -1, head);
	i = -1;
	while (tmp_env[++i])
		ft_free((void *)&tmp_env[i], ft_strlen(tmp_env[i]));
	free(tmp_env);
	return (env_len);
}

int	search_envvar(char *envvar, char **env)
{
	int	i;
	int	hidden_flag;

	hidden_flag = 0;
	i = 0;
	if (!envvar)
		return (0);
	if (envvar[0] == '.')
		envvar++;
	while (env[i])
	{
		if ((env[i][0] == '.' || env[i][ft_strlen(envvar)] == '=')
			&& (!ft_strncmp(env[i], envvar, ft_strlen(envvar))
			|| (env[i][0] == '.'
				&& !ft_strncmp(env[i] + 1, envvar, ft_strlen(envvar)))))
			return (i);
		i++;
	}
	return (0);
}