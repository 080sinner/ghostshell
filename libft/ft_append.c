/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_append.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 20:23:10 by fbindere          #+#    #+#             */
/*   Updated: 2021/11/22 22:31:01 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_append(char *line, char c)
{
	int		length;
	int		i;
	char	*longer;

	if (line == NULL)
		return (NULL);
	length = ft_strlen(line);
	longer = ft_calloc(length + 2, sizeof(char));
	if (longer == NULL)
	{
		free(line);
		return (NULL);
	}
	i = 0;
	while (line[i] != '\0')
	{
		longer[i] = line[i];
		i++;
	}
	longer[i] = c;
	free(line);
	return (longer);
}
