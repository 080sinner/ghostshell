/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_freestrarray.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 23:11:23 by eozben            #+#    #+#             */
/*   Updated: 2021/11/24 23:11:42 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_free_strarray(char **split)
{
	int	i;

	i = 0;
	while (split[i] != NULL)
		free(split[i++]);
	free(split);
	return (0);
}
