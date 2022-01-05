/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 21:52:34 by fbindere          #+#    #+#             */
/*   Updated: 2022/01/06 00:53:27 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int check_newline_flag(int *i, char **args)
{
	int j;
	int newline_flag;
	newline_flag = 1;
	while (args[*i] && args[*i][0] == '-')
	{
		j = 1;
		while (args[*i][j])
		{
			if (args[*i][j] == 'n')
				j++;
			else
				break ;
		}
		if (args[*i][j] == '\0')
			newline_flag = 0;
		else
			break ;
		*i += 1;
	}
	return (newline_flag);
}

int echo(char **args)
{
	int i;
	int newline_flag;
	newline_flag = 1;
	i = 1;
	if (!args[i])
	{
		write(1, "\n", 1);
		return (0);
	}
	newline_flag = check_newline_flag(&i, args);
	while (args[i])
		printf("%s ", args[i++]);
	if (newline_flag)
		printf("\n");
	return (0);
}
