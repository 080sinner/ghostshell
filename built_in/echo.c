/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 21:52:34 by fbindere          #+#    #+#             */
/*   Updated: 2022/01/05 21:52:53 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	echo(char **args)
{
	int	i;

	i = 1;
	if (!args[i])
	{
		write(1, "\n", 1);
		return (0);
	}
	while (!ft_strncmp(args[i], "-n", 2))
		i++;
	while (args[i])
	{
		printf("%s ", args[i]);
		i++;
	}
	if (ft_strncmp(args[1], "-n", 2))
		printf("\n");
	return (0);
}
