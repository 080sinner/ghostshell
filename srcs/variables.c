/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 23:18:50 by eozben            #+#    #+#             */
/*   Updated: 2022/01/04 23:17:11 by eozben           ###   ########.fr       */
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
		return (-1);
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
	if (i == -1)
		return (-1);
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

t_tok	*create_new_tok(t_tok **headtok)
{
	t_tok	*new;

	new = ft_dll_append_tok(headtok);
	if (!new)
		return (NULL);
	new->data = ft_strdup("");
	if (!new->data)
	{
		free(new);
		return (NULL);
	}
	return (new);
}

t_tok	*expand_variable(char *data, char *varcontent, int tmp)
{
	t_tok	*headtok;
	t_tok	*new;
	int		ret;

	ret = 0;
	headtok = NULL;
	while (*data != '\0' || varcontent)
	{
		if (varcontent || !headtok)
		{
			if (append_general_variable(new, &varcontent) || !headtok)
			{
				new = create_new_tok(&headtok);
				if (!new)
					return (NULL);
			}
			continue ;
		}
		else if (*data == DQUOTED_STATE || *data == GENERAL_STATE)
		{
			tmp = *data;
			ret = read_variable(data, &varcontent, headtok);
			if (ret == -1)
				return (NULL);
			data += ret;
			if (!varcontent)
				return (NULL);
			if (tmp == DQUOTED_STATE && append_dquoted_variable(&varcontent, new) == -1)
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
