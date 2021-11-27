/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 23:06:32 by eozben            #+#    #+#             */
/*   Updated: 2021/11/27 20:15:55 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	error_close_dir(DIR *dir)
{
	closedir(dir);
	return (0);
}

static int	search_wildcard(char **filename, char *data, char **split)
{
	int	i;

	i = 0;
	while (split[i] != NULL)
	{
		if (data[0] != -42 && i == 0)
		{
			if (ft_strncmp(split[i], *filename, ft_strlen(split[i])))
				return (ft_free_strarray(split));
			*filename += ft_strlen(split[i]);
			i++;
			continue ;
		}
		*filename = ft_strnstr(*filename, split[i], ft_strlen(*filename));
		if (*filename == NULL)
			return (ft_free_strarray(split));
		*filename += ft_strlen(split[i]);
		if (ft_strnstr(*filename, split[i], ft_strlen(*filename)))
			continue ;
		i++;
	}
	return (1);
}

static int	match_wildcard(char *filename, char *data)
{
	char	**split;

	split = ft_split(data, -42);
	if (split == NULL)
		return (0);
	if (*split == NULL)
	{
		ft_free_strarray(split);
		return (1);
	}
	if (!search_wildcard(&filename, data, split))
		return (0);
	ft_free_strarray(split);
	if (*filename == '\0' || data[ft_strlen(data) - 1] == -42)
		return (1);
	return (0);
}

static int	wildcard_expansion(t_tok **token, DIR *dir, int checkvalue)
{
	t_tok			*new;
	struct dirent	*entity;

	new = NULL;
	while (1)
	{
		entity = readdir(dir);
		if (entity == NULL)
			break ;
		if ((entity->d_name[0] == '.' && (*token)->data[0] != '.')
			|| (!match_wildcard(entity->d_name, (*token)->data)))
			continue ;
		new = ft_dll_append_tok(token);
		if (!new)
			return (error_close_dir(dir));
		new->data = ft_strdup(entity->d_name);
		if (!new->data)
			return (error_close_dir(dir));
		checkvalue = 1;
	}
	if (new != NULL)
		ft_free((void *)&(*token)->data, ft_strlen((*token)->data));
	closedir(dir);
	return (checkvalue);
}

int	handle_wildcards(t_tok **new, __unused t_tok **head)
{
	if (!ft_strchr((*new)->data, -42))
		return (0);
	if (!wildcard_expansion(new, opendir("."), 0))
	{
		while (ft_strchr((*new)->data, -42))
			*(ft_strchr((*new)->data, -42)) = '*';
	}
	else
	{
		if ((*new)->data != NULL)
			free((*new)->data);
		free(detach_tok(head, *new));
	}
	return (0);
}
