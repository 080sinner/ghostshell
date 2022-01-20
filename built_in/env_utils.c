/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 21:46:11 by fbindere          #+#    #+#             */
/*   Updated: 2022/01/21 00:12:46 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_getenv(char *envvar, char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], envvar, ft_strlen(envvar)))
		{
			if (*(env[i] + ft_strlen(envvar)) == '=')
				return (env[i] + ft_strlen(envvar) + 1);
		}
		i++;
	}
	return (NULL);
}

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

	i = 0;
	if (!envvar)
		return (0);
	if (envvar[0] == '.')
		envvar++;
	while (env[i])
	{
		if ((env[i][0] == '.' || env[i][ft_strlen(envvar)] == '=') &&
			(!ft_strncmp(env[i], envvar, ft_strlen(envvar))
			|| (env[i][0] == '.'
			&& !ft_strncmp(env[i] + 1, envvar, ft_strlen(envvar)))))
			return (i);
		i++;
	}
	return (-1);
}

void	ft_copy_env(char **environ, int skip_var, t_node **head)
{
	int			i;
	int			x;

	i = 0;
	while (environ[i])
		i++;
	g_utils.environment = ft_calloc(i + 2, sizeof(char *));
	if (!g_utils.environment)
		ft_exit(g_utils.exit_status, head);
	i = 0;
	x = i;
	while (environ[i])
	{
		if (skip_var == -1 || x != skip_var)
		{
			g_utils.environment[i] = ft_strdup(environ[x]);
			i++;
		}
		x++;
	}
}
