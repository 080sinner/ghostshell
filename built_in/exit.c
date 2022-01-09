/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 21:52:43 by fbindere          #+#    #+#             */
/*   Updated: 2022/01/09 19:24:29 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	exit_builtin(t_node **head, int process_lvl)
{
	char	*username;

	if (process_lvl == 0)
	{
		ft_putstr_fd("Frighten no more ", 2);
		username = ft_strdup(ft_getenv("USER", g_utils.environment));
		if (username)
		{
			ft_putstr_fd(username, 2);
			free(username);
		}
		else
			ft_putstr_fd("visitor", 2);
		ft_putstr_fd("! You escaped the GHOSTSHELL!\n", 2);
	}
	free_nodes(head);
	ft_free_strarray(g_utils.environment);
	exit(EXIT_SUCCESS);
}
