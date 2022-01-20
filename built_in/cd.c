/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 21:50:09 by fbindere          #+#    #+#             */
/*   Updated: 2022/01/20 18:47:46 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	cd_error(char *path)
{
	ft_putstr_fd("Haunting mistake: cd: ", 2);
	ft_putstr_fd(path, 2);
	if (errno == 13)
		ft_putendl_fd(": permission denied", 2);
	else
		ft_putendl_fd(": no such file or directory", 2);
	return (1);
}

int	change_dir(char *path)
{
	int		ret;
	char	*old_pwd;
	char	*new_pwd;

	ret = ERROR;
	if (path == NULL || *path == '~')
		path = ft_getenv("HOME", g_utils.environment);
	if (!path)
	{
		ft_putendl_fd("cd: HOME not set", 2);
		return (1);
	}
	ret = chdir(path);
	if (ret == ERROR)
		return (cd_error(path));
	else
	{
		new_pwd = getcwd(NULL, 0);
		old_pwd = ft_getenv("PWD", g_utils.environment);
		ft_strlcpy(old_pwd, new_pwd, ft_strlen(new_pwd) + 1);
		ft_free((void *)&new_pwd, ft_strlen(new_pwd));
	}
	return (ret);
}
