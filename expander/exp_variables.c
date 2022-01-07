/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_variables.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 23:18:50 by eozben            #+#    #+#             */
/*   Updated: 2022/01/07 16:41:46 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	dquoted_var(char **varcontent, t_tok *new)
{
	char	*tmp;

	if (*varcontent == NULL)
		return (1);
	tmp = new->data;
	new->data = ft_strjoin(new->data, *varcontent);
	free(tmp);
	if (!new->data)
		return (0);
	if (*varcontent)
		free(*varcontent);
	*varcontent = NULL;
	return (1);
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

int	read_variable(char *data, char **varcontent)
{
	int		i;
	char	*varname;

	i = read_variable_name(data, &varname) + 1;
	if (i == ERROR)
		return (ERROR);
	*varcontent = ft_strdup(ft_getenv(varname, g_utils.environment));
	ft_free((void *) &varname, ft_strlen(varname));
	return (i);
}

int	general_variable(t_tok *new, char **varcontent)
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

int	get_varcontent(char **varcontent, char **data)
{
	int		ret;
	int		tmp;

	ret = 0;
	tmp = **data;
	ret = read_variable(*data, varcontent);
	if (ret == ERROR)
		return (0);
	*data += ret;
	return (tmp);
}

t_tok	*expand_variable(char *data, char *varcontent)
{
	t_tok	*headtok;
	t_tok	*new;
	int		var_type;

	headtok = NULL;
	while (*data != '\0' || varcontent)
	{
		if (varcontent || !headtok)
		{
			if (general_variable(new, &varcontent) || !headtok)
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
			var_type = get_varcontent(&varcontent, &data);
			if (var_type == DQUOTED_STATE && !dquoted_var(&varcontent, new))
				return (NULL);
			else if (!var_type)
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
