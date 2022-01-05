/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 23:18:50 by eozben            #+#    #+#             */
/*   Updated: 2022/01/06 00:12:45 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	append_dquoted_variable(char **varcontent, t_tok *new)
{
	char	*tmp;

	if (*varcontent == NULL)
		return (0);
	tmp = new->data;
	new->data = ft_strjoin(new->data, *varcontent);
	free(tmp);
	if (!new->data)
		return (ERROR);
	if (*varcontent)
		free(*varcontent);
	*varcontent = NULL;
	return (0);
}

int	read_variable_name(char *data, char **varname)
{
	int		i;

	i = 0;
	data += 1;
	while (data[i] != END && data[i] != '\0')
		i++;
	*varname = ft_substr(data, 0, i);
	if (!*varname)
		return (-1);
	return (i + 1);
}

int	read_variable(char *data, char **varcontent, t_tok *headtok)
{
	int		i;
	char	*varname;

	i = read_variable_name(data, &varname) + 1;
	if (i == ERROR)
		return (ERROR);
	*varcontent = ft_strdup(ft_getenv(varname, g_utils.environment));
	if (!*varcontent)
		free_toks(&headtok);
	ft_free((void *) &varname, ft_strlen(varname));
	return (i);
}

int	append_general_variable(t_tok *new, char **varcontent)
{
	static int	i;

	while (*varcontent && (*varcontent)[i] != '\0')
	{
		if (check_whitespace((*varcontent)[i]))
		{
			while (check_whitespace((*varcontent)[i]))
				i++;
		}
		if ((*varcontent)[i] == '*')
			(*varcontent)[i] = -42;
		new->data = ft_append(new->data, (*varcontent)[i]);
		i++;
	}
	if (*varcontent)
		free(*varcontent);
	*varcontent = NULL;
	i = 0;
	return (0);
}

int append_quoted_variable(char **varcontent, char **data, t_tok **headtok, t_tok *new)
{
	int		ret;
	int		tmp;

	ret = 0;
	tmp = **data;
	ret = read_variable(*data, varcontent, *headtok);
	if (ret == ERROR)
		return (0);
	*data += ret;
	if (!*varcontent)
		return (0);
	if (tmp == DQUOTED_STATE && append_dquoted_variable(varcontent, new) == ERROR)
		return (0);
	return (1);
}

t_tok	*expand_variable(char *data, char *varcontent)
{
	t_tok	*headtok;
	t_tok	*new;

	headtok = NULL;
	while (*data != '\0' || varcontent)
	{
		if (varcontent || !headtok)
		{
			if (append_general_variable(new, &varcontent) || !headtok)
			{
				new = create_new_tok();
				if (!new)
					return (NULL);
				ft_dll_attach_tok(&headtok, new);
			}
			continue ;
		}
		else if (*data == DQUOTED_STATE || *data == GENERAL_STATE)
		{
			if (!append_quoted_variable(&varcontent, &data, &headtok, new))
				return (NULL);
		}
		else if (!varcontent)
		{
			new->data = ft_append(new->data, *data);
			if (!new->data)
				return (NULL);
			data++;
		}
	}
	return (headtok);
}
