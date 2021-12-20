/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/18 17:20:43 by eozben            #+#    #+#             */
/*   Updated: 2021/12/20 19:46:50 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	change_dir(char *path)
{
	int		ret;
	char	*err_msg;

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
	getcwd(ft_getenv("PWD", g_utils.environment), 1);
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

int	check_builtin(t_node *command)
{
	int	exit;

	exit = -1;
	if (command->cmd_arr && command->cmd_arr[0])
	{
		if (ft_strcmp(command->cmd_arr[0], "cd"))
			exit = change_dir(command->cmd_arr[1]);
		// else if (ft_strcmp(command->cmd_arr[0], "echo"))
		// 	printf("echo\n");
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