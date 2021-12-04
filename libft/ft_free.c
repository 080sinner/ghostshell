/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 18:41:28 by eozben            #+#    #+#             */
/*   Updated: 2021/12/04 17:36:43 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// frees the given ptr and sets n bytes to zero (strlen of ptr if its a string)
int	ft_free(void **ptr, size_t n)
{
	if (*ptr != NULL)
	{
		ft_bzero(*ptr, n);
		free(*ptr);
		*ptr = NULL;
	}
	return (0);
}
