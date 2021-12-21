/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/18 17:20:43 by eozben            #+#    #+#             */
/*   Updated: 2021/12/21 17:20:37 by eozben           ###   ########.fr       */
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

int	echo(char **args)
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

int	print_pwd(void)
{
	printf("%s\n", ft_getenv("PWD", g_utils.environment));
	return (0);
}

int	search_envvar(char *envvar, char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], envvar, ft_strlen(envvar)))
			return (i);
		i++;
	}
	return (0);
}

int	create_new_env(char **env)
{
	char	**tmp_env;
	int		i;
	int		env_len;

	tmp_env = env;
	i = -1;
	while (env[++i])
		tmp_env[i] = env[i];
	env_len = i;
	ft_copy_env(g_utils.environment);
	i = -1;
	while (tmp_env[++i])
		ft_free((void *)&tmp_env[i], ft_strlen(tmp_env[i]));
	free(tmp_env);
	return (env_len);
}

int	export(t_node *command)
{
	int		env_len;
	char	*envvar[2];

	env_len = create_new_env(g_utils.environment);
	envvar[ENV_VAR_CONTENT] = ft_strdup(ft_strchr(command->cmd_arr[1], '='));
	if (!envvar[ENV_VAR_CONTENT])
		return (0);
	*(ft_strchr(command->cmd_arr[1], '=')) = '\0';
	envvar[ENV_VAR_NAME] = command->cmd_arr[1];
	if (!search_envvar(envvar[ENV_VAR_NAME], g_utils.environment))
		g_utils.environment[env_len] = ft_strjoin(envvar[0], envvar[1]);
	else
	{
		env_len = search_envvar(envvar[ENV_VAR_NAME], g_utils.environment);
		free(g_utils.environment[env_len]);
		g_utils.environment[env_len] = ft_strjoin(envvar[0], envvar[1]);
	}
	ft_free((void *)&envvar[ENV_VAR_CONTENT], ft_strlen(envvar[1]));
	return (0);
}

int	check_builtin(t_node *command, t_node **head)
{
	int	exit;

	exit = -1;
	parse_command(command, head);
	if (command->cmd_arr && command->cmd_arr[0])
	{
		if (ft_strcmp(command->cmd_arr[0], "cd"))
			exit = change_dir(command->cmd_arr[1]);
		else if (ft_strcmp(command->cmd_arr[0], "echo"))
			exit = echo(command->cmd_arr);
		else if (ft_strcmp(command->cmd_arr[0], "pwd"))
			exit = print_pwd();
		else if (ft_strcmp(command->cmd_arr[0], "export"))
			exit = export(command);
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
