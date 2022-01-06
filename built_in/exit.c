/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 21:52:43 by fbindere          #+#    #+#             */
/*   Updated: 2022/01/06 20:07:27 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	exit_builtin(t_node **head)
{
	free_nodes(head);
	ft_free_strarray(g_utils.environment);
	exit(EXIT_SUCCESS);
}
