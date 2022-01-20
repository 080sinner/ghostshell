/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 16:28:42 by eozben            #+#    #+#             */
/*   Updated: 2022/01/20 18:47:49 by eozben           ###   ########.fr       */
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

static int	is_direct_path(char *data, char **paths)
{
	if (!paths)
		return (1);
	if (ft_strchr(data, '/'))
		return (1);
	if (ft_strcmp(data, ".") || ft_strcmp(data, ".."))
		return (1);
	return (0);
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

static int	cmd_not_found(char **paths, char *command)
{
	struct stat	dir;
	int			dir_ret;

	dir_ret = stat(command, &dir);
	if (dir_ret != ERROR && dir.st_mode & S_IFDIR
		&& !ft_strcmp(command, ".") && !ft_strcmp(command, ".."))
	{
		ft_putstr_fd("Haunting mistake: ", 2);
		ft_putstr_fd(command, 2);
		ft_putendl_fd(" is a directory", 2);
		g_utils.exit_status = 126;
	}
	else if (is_direct_path(command, paths))
	{
		ft_putendl_fd("Haunting mistake: No such file or directory", 2);
		g_utils.exit_status = 127;
	}
	else
	{
		ft_putstr_fd("Haunting mistake: command not found: ", 2);
		ft_putendl_fd(command, 2);
		g_utils.exit_status = 127;
	}
	ft_free_strarray(paths);
	return (ERROR);
}

int	get_cmd_path(t_node *command)
{
	char		**paths;
	char		*data;
	struct stat	s;

	paths = NULL;
	if (!command->args || !command->args->data)
		return (ERROR);
	data = command->args->data;
	if (check_builtin(command->args))
		return (1);
	paths = ft_split(ft_getenv("PATH", g_utils.environment), ':');
	if (is_direct_path(data, paths))
	{
		if (!access(data, F_OK) && !stat(data, &s) && !(s.st_mode & S_IFDIR))
		{
			command->cmdpath = ft_strdup(data);
			return (1);
		}
		return (cmd_not_found(paths, command->args->data));
	}
	if (append_slash(paths, "/") == ERROR)
		return (ERROR);
	if (cmdpath(command, paths) == ERROR)
		return (cmd_not_found(paths, command->args->data));
	return (1);
}
