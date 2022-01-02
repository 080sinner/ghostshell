/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/18 17:20:43 by eozben            #+#    #+#             */
/*   Updated: 2022/01/02 22:58:23 by fbindere         ###   ########.fr       */
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
	if (!path)
	{
		ft_putendl_fd("cd: HOME not set", 2);
		return (1);
	}
	ret = chdir(path);
	if (ret == -1)
	{
		err_msg = ft_strjoin("cd: ", path);
		perror(err_msg);
		ft_free((void *)&err_msg, ft_strlen(err_msg));
		return (1);
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

int	print_env(int declare_flag)
{
	int	i;

	i = 0;
	if (declare_flag)
	{
		while (g_utils.environment[i])
		{
			if (g_utils.environment[i][0] == '.')
				printf("declare -x %s\n", g_utils.environment[i++] + 1);
			else
				printf("declare -x %s\n", g_utils.environment[i++]);
		}
	}
	else
	{
		while (g_utils.environment[i])
		{
			if (g_utils.environment[i][0] != '.')
				printf("%s\n", g_utils.environment[i]);
			i++;
		}
	}
	return (0);
}

int	echo(char **args)
{
	int	i;

	i = 1;
	if (!args[i])
	{
		write(1, "\n", 1);
		return (0);
	}
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
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (1);
	printf("%s\n", pwd);
	free(pwd);
	return (0);
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
		envvar[ENV_VAR_CONTENT] = ft_strdup(ft_strchr(command->cmd_arr[i], '='));
		if (!envvar[ENV_VAR_CONTENT])
		{
			envvar[ENV_VAR_CONTENT] = ft_strdup("");
			envvar[ENV_VAR_NAME] = command->cmd_arr[i];
		}
		else
		{
			env_flag = 1;
			*(ft_strchr(command->cmd_arr[i], '=')) = '\0';
			envvar[ENV_VAR_NAME] = command->cmd_arr[i];
		}
		if (check_valid_var_name(envvar[ENV_VAR_NAME]))
		{
			printf("export: '%s' not a valid identifier\n", envvar[ENV_VAR_NAME]);
			ft_free((void *)&envvar[ENV_VAR_CONTENT], ft_strlen(envvar[1]));
			ret = 1;
			envvar[ENV_VAR_NAME] = NULL;
		}
		if (env_flag == 1 && !search_envvar(envvar[ENV_VAR_NAME], g_utils.environment))
			g_utils.environment[env_len] = ft_strjoin(envvar[0], envvar[1]);
		else if (env_flag == 1 && envvar[ENV_VAR_NAME])
		{
			env_len = search_envvar(envvar[ENV_VAR_NAME], g_utils.environment);
			free(g_utils.environment[env_len]);
			g_utils.environment[env_len] = ft_strjoin(envvar[0], envvar[1]);
		}
		else if (!search_envvar(envvar[ENV_VAR_NAME], g_utils.environment))
			g_utils.environment[env_len] = ft_strjoin(".", envvar[ENV_VAR_NAME]);
		ft_free((void *)&envvar[ENV_VAR_CONTENT], ft_strlen(envvar[1]));
	}
	return (ret);
}

int unset(t_node *command, t_node **head)
{
	int		var_index;
	char	**tmp_env;
	int		i;
	int		count;
	int		ret;

	ret = 0;
	count = 0;
	while (command->cmd_arr[++count])
	{
		tmp_env = g_utils.environment;
		i = -1;
		while (++i)
			tmp_env[i] = g_utils.environment[i];
		if (check_valid_var_name(command->cmd_arr[count]))
		{
			printf("unset: '%s' not a valid identifier\n", command->cmd_arr[count]);
			ret = 1;
		}
		else
		{
			var_index = search_envvar(command->cmd_arr[count], g_utils.environment);
			ft_copy_env(g_utils.environment, var_index, head);
			i = -1;
			while (tmp_env[++i])
				ft_free((void *)&tmp_env[i], ft_strlen(tmp_env[i]));
			free(tmp_env);
		}
	}
	return (ret);
}

int	execute_builtin (t_node *command, t_node **head)
{
	if (command->cmd_arr && command->cmd_arr[0])
	{
		if (ft_strcmp(command->cmd_arr[0], "cd"))
			return (change_dir(command->cmd_arr[1]));
		else if (ft_strcmp(command->cmd_arr[0], "echo"))
			return (echo(command->cmd_arr));
		else if (ft_strcmp(command->cmd_arr[0], "pwd"))
			return (print_pwd());
		else if (ft_strcmp(command->cmd_arr[0], "export"))
			return (export(command, head));
		else if (ft_strcmp(command->cmd_arr[0], "unset"))
			return (unset(command, head));
		else if (ft_strcmp(command->cmd_arr[0], "env"))
			return (print_env(0));
	}
	return (ERROR);
}

// int	check_builtin (t_node *command)
// {
// 	char	*builtins[6];
// 	int		i;

// 	if (command->cmd_arr == NULL)
// 		return (0);
// 	ft_striteri(command->cmd_arr[0], ft_tolower);
// 	builtins[0] = "cd";
// 	builtins[1] = "echo";
// 	builtins[2] = "pwd";
// 	builtins[3] = "export";
// 	builtins[4] = "env";
// 	builtins[5] = "unset";
// 	i = 0;
// 	if (command->cmd_arr && command->cmd_arr[0])
// 	{
// 		while (i <= 5)
// 		{
// 			if (ft_strcmp(command->cmd_arr[0], builtins[i]))
// 				return (1);
// 			i++;
// 		}
// 	}
// 	return (0);
// }


int	check_builtin (t_tok *command)
{
	char	*builtins[6];
	int		i;

	if (command == NULL)
		return (0);
	while (command->type != COMMAND)
		command = command->next;
	if (command->previous && command->previous->type != COMMAND)
		command = command->next;
	ft_striteri(command->data, ft_tolower);
	builtins[0] = "cd";
	builtins[1] = "echo";
	builtins[2] = "pwd";
	builtins[3] = "export";
	builtins[4] = "env";
	builtins[5] = "unset";
	i = 0;
	while (i <= 5)
	{
		if (ft_strcmp(command->data, builtins[i]))
			return (1);
		i++;
	}
	return (0);
}