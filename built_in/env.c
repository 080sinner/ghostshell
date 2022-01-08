/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 23:05:59 by fbindere          #+#    #+#             */
/*   Updated: 2022/01/08 18:02:02 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	print_env(int declare_flag)
{
	int	i;

	i = 0;
	if (declare_flag)
	{
		while (g_utils.environment[i])
		{
			if (g_utils.environment[i][0] == '.')
				printf("declare -x %s\n", g_utils.environment[i++] + 1);
			else
				printf("declare -x %s\n", g_utils.environment[i++]);
		}
	}
	else
	{
		while (g_utils.environment[i])
		{
			if (g_utils.environment[i][0] != '.')
				printf("%s\n", g_utils.environment[i]);
			i++;
		}
	}
	return (0);
}
