/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getpath.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 16:28:42 by eozben            #+#    #+#             */
/*   Updated: 2021/12/20 17:00:35 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void append_slash(char **paths, char *appendage)
{
	int		i;
	char	*temp;
	i = 0;
	while (paths[i] != NULL)
	{
		temp = paths[i];
		paths[i] = ft_strjoin(paths[i], appendage);
		if (paths[i] == NULL)
		{
			paths[i] = temp;
			ft_free_strarray(paths);
			exit(EXIT_FAILURE);
		}
		free(temp);
		i++;
	}
	return ;
}

static void cmdpath(t_node *command, char **paths)
{
	int i;
	i = 0;
	if (command->cmdpath)
		ft_free((void *)&command->cmdpath, ft_strlen(command->cmdpath));
	while (paths[i])
	{
		command->cmdpath = ft_strjoin(paths[i], command->args->data);
		if (!command->cmdpath)
			exit(EXIT_FAILURE);
		if (access(command->cmdpath, F_OK) == 0)
			return ;
		ft_free((void *)&command->cmdpath, ft_strlen(command->cmdpath));
		i++;
	}
	// errno = NOCOMMAND;
	// ft_error(input->args[0], input);
}

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

void	get_cmd_path(t_node *command)
{
	char	**paths;
	if (!command->args)
		return ;
	if (access(command->args->data, F_OK) == 0)
	{
		command->cmdpath = ft_strdup(command->args->data);
		return ;
	}
	paths = ft_split(ft_getenv("PATH", g_utils.environment), ':');
	if (!paths)
		exit(EXIT_FAILURE);
	append_slash(paths, "/");
	cmdpath(command, paths);
	ft_free_strarray(paths);
	// errno = NOCOMMAND;
	// ft_error(input->args[0], input);
}
