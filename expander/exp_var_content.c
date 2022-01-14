/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_var_content.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/07 22:20:00 by fbindere          #+#    #+#             */
/*   Updated: 2022/01/14 17:34:32 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	dquoted_var(char **varcontent, t_tok *new, int var_type)
{
	char	*tmp;

	if (*varcontent == NULL || var_type != DQUOTED_STATE)
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

int	general_variable(t_tok *new, char **varcontent, char *data)
{
	static int	i;

	while (*varcontent && *(*varcontent + i) != '\0')
	{
		if (check_whitespace(*(*varcontent + i)))
		{
			while (check_whitespace(*(*varcontent + i)))
				i++;
			if (new->data[0])
			{
				if (*data == '\0' && *(*varcontent + i) == '\0')
					break ;
				return (1);
			}
		}
		if (*(*varcontent + i) == '*')
			*(*varcontent + i) = -42;
		new->data = ft_append(new->data, *(*varcontent + i));
		i++;
	}
	if (*varcontent)
		free(*varcontent);
	*varcontent = NULL;
	i = 0;
	return (0);
}
