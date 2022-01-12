/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 16:28:42 by eozben            #+#    #+#             */
/*   Updated: 2022/01/12 21:57:01 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	append_slash(char **paths, char *appendage)
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
			return (ERROR);
		}
		free(temp);
		i++;
	}
	return (1);
}

static int	cmdpath(t_node *command, char **paths)
{
	int	i;

	i = 0;
	if (command->cmdpath)
		ft_free((void *)&command->cmdpath, ft_strlen(command->cmdpath));
	if (command->args->data[0] == '\0')
		return (ERROR);
	while (paths[i])
	{
		command->cmdpath = ft_strjoin(paths[i], command->args->data);
		if (!command->cmdpath)
			return (ERROR);
		if (access(command->cmdpath, F_OK) == 0)
			return (1);
		ft_free((void *)&command->cmdpath, ft_strlen(command->cmdpath));
		i++;
	}
	return (ERROR);
}

static int	cmd_not_found(char **paths, t_node *command)
{
	ft_free_strarray(paths);
	if (ft_strcmp(command->args->data, "."))
		ft_putendl_fd("error: .: filename argument required", 2);
	else
	{
		ft_putstr_fd("error: command not found: ", 2);
		ft_putendl_fd(command->args->data, 2);
	}
	g_utils.exit_status = 127;
	return (ERROR);
}

static int	is_directory(t_node *command)
{
	if (ft_strcmp(command->args->data, "."))
		return (1);
	//if (command->args->data[ft_strlen(command->args->data) - 1])
		//return (1);
	return (0);
}

int	get_cmd_path(t_node *command)
{
	char	**paths;

	paths = NULL;
	if (!command->args)
		return (ERROR);
	if (check_builtin(command->args))
		return (1);
	if (ft_strchr(command->args->data, '/') || ft_strcmp(command->args->data, "."))
	{
		if (!access(command->args->data, F_OK) && !is_directory(command))
		{
			command->cmdpath = ft_strdup(command->args->data);
			return (1);
		}
		return(cmd_not_found(paths, command));
	}
	paths = ft_split(ft_getenv("PATH", g_utils.environment), ':');
	if (!paths  || append_slash(paths, "/") == ERROR)
		return (ERROR);
	if (cmdpath(command, paths) == ERROR)
		return (cmd_not_found(paths, command));
	return (1);
}
