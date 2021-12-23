/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 23:18:50 by eozben            #+#    #+#             */
/*   Updated: 2021/12/23 22:53:31 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	append_dquoted_variable(char **varcontent, t_tok *new, t_node **head)
{
	char	*tmp;

	if (*varcontent == NULL)
		return (0);
	tmp = new->data;
	new->data = ft_strjoin(new->data, *varcontent);
	if (!new->data)
		ft_exit(EXIT_FAILURE, head);
	free(tmp);
	if (*varcontent)
		free(*varcontent);
	*varcontent = NULL;
	return (0);
}

int	read_variable_name(char *data, t_node **head, char **varname)
{
	int		i;

	i = 0;
	data += 1;
	while (data[i] != END && data[i] != '\0')
		i++;
	*varname = ft_substr(data, 0, i);
	if (!*varname)
		ft_exit(EXIT_FAILURE, head);
	return (i + 1);
}

int	read_variable(char *data, char **varcontent, t_node **head, t_tok *headtok)
{
	int		i;
	char	*varname;

	i = read_variable_name(data, head, &varname) + 1;
	*varcontent = ft_strdup(ft_getenv(varname, g_utils.environment));
	if (!*varcontent)
		free_toks(&headtok);
	ft_free((void *) &varname, ft_strlen(varname));
	return (i);
}

int	append_general_variable(t_tok *new, char **varcontent, t_node **head)
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
		new->data = ft_append(new->data, (*varcontent)[i], head);
		i++;
	}
	if (*varcontent)
		free(*varcontent);
	*varcontent = NULL;
	i = 0;
	return (0);
}

t_tok	*create_new_tok(t_tok **headtok, t_node **head)
{
	t_tok	*new;

	new = ft_dll_append_tok(headtok, head);
	new->data = ft_strdup("");
	if (!new->data)
		ft_exit(EXIT_FAILURE, head);
	return (new);
}

t_tok	*expand_variable(char *data, t_node **head, char *varcontent, int tmp)
{
	t_tok	*headtok;
	t_tok	*new;

	headtok = NULL;
	while (*data != '\0' || varcontent)
	{
		if (varcontent || !headtok)
		{
			if (append_general_variable(new, &varcontent, head) || !headtok)
				new = create_new_tok(&headtok, head);
			continue ;
		}
		else if (*data == DQUOTED_STATE || *data == GENERAL_STATE)
		{
			tmp = *data;
			data += read_variable(data, &varcontent, head, headtok);
			if (!varcontent)
				return (NULL);
			if (tmp == DQUOTED_STATE)
				append_dquoted_variable(&varcontent, new, head);
		}
		else if (!varcontent)
		{
			new->data = ft_append(new->data, *data, head);
			data++;
		}
	}
	return (headtok);
}
