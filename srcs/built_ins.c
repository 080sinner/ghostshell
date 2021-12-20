/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/18 17:20:43 by eozben            #+#    #+#             */
/*   Updated: 2021/12/20 20:38:54 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	change_dir(char *path)
{
	int		ret;
	char	*err_msg;
	char	*old_pwd;
	char	*new_pwd;

	err_msg = NULL;
	ret = -1;
	if (path == NULL || *path == '~')
		path = ft_getenv("HOME", g_utils.environment);
	ret = chdir(path);
	if (ret == -1)
	{
		err_msg = ft_strjoin("cd: ", path);
		perror(err_msg);
		ft_free((void *)&err_msg, ft_strlen(err_msg));
	}
	else
	{
		new_pwd = getcwd(NULL, 0);
		old_pwd = ft_getenv("PWD", g_utils.environment);
		ft_strlcpy(old_pwd, new_pwd, ft_strlen(new_pwd) + 1);
		ft_free((void *)&new_pwd, ft_strlen(new_pwd));
	}
	return (ret);
}

int	print_env(void)
{
	int	i;

	i = 0;
	while (g_utils.environment[i])
		printf("%s\n", g_utils.environment[i++]);
	return (0);
}

int	bi_echo(char **args)
{
	int	i;

	i = 1;
	while (!ft_strncmp(args[i], "-n", 2))
		i++;
	while (args[i])
	{
		printf("%s ", args[i]);
		i++;
	}
	if (ft_strncmp(args[1], "-n", 2))
		printf("\n");
	return (0);
}

int	check_builtin(t_node *command)
{
	int	exit;

	exit = -1;
	if (command->cmd_arr && command->cmd_arr[0])
	{
		if (ft_strcmp(command->cmd_arr[0], "cd"))
			exit = change_dir(command->cmd_arr[1]);
		else if (ft_strcmp(command->cmd_arr[0], "echo"))
			exit = bi_echo(command->cmd_arr);
		// else if (ft_strcmp(command->cmd_arr[0], "pwd"))
		// 	printf("pwd\n");
		// else if (ft_strcmp(command->cmd_arr[0], "export"))
		// 	printf("export\n");
		// else if (ft_strcmp(command->cmd_arr[0], "unset"))
		// 	printf("unset\n");
		else if (ft_strcmp(command->cmd_arr[0], "env"))
			exit = print_env();
		else
			return (0);
		return (1);
	}
	return (exit);
}