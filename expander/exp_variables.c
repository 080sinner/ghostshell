/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_variables.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 23:18:50 by eozben            #+#    #+#             */
/*   Updated: 2022/01/14 17:15:14 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
	if (ft_strcmp(varname, "?"))
		*varcontent = ft_itoa(g_utils.exit_status);
	else
		*varcontent = ft_strdup(ft_getenv(varname, g_utils.environment));
	ft_free((void *) &varname, ft_strlen(varname));
	return (i);
}

static int	get_varcontent(char **varcontent, char **data)
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

static t_tok	*append_char(t_tok *new, char **data)
{
	new->data = ft_append(new->data, **data);
	if (!new->data)
		return (NULL);
	(*data)++;
	return (new);
}

t_tok	*expand_variable(char *data, char *varcontent, int var_type)
{
	t_tok	*tok[2];

	tok[HEAD] = NULL;
	while (*data != '\0' || varcontent)
	{
		if (!tok[HEAD] || (var_type == GENERAL_STATE && varcontent))
		{
			if (general_variable(tok[NEW], &varcontent, data) || !tok[HEAD])
			{
				tok[NEW] = create_new_tok();
				if (!tok[NEW])
					return (NULL);
				ft_dll_attach_tok(&tok[HEAD], tok[NEW]);
			}
		}
		else if (*data == DQUOTED_STATE || *data == GENERAL_STATE)
		{
			var_type = get_varcontent(&varcontent, &data);
			if (!var_type || !dquoted_var(&varcontent, tok[NEW], var_type))
				return (NULL);
		}
		else if (!varcontent && !append_char(tok[NEW], &data))
			return (NULL);
	}
	return (tok[HEAD]);
}
