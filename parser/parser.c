/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 17:27:51 by fbindere          #+#    #+#             */
/*   Updated: 2022/01/17 16:22:40 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	parse_command(t_node *current, t_node **head)
{
	if (!current)
		return (ERROR);
	if (expander(current) == ERROR)
		return (ERROR);
	if (set_redir(current, head, current->previous, current->next) == ERROR)
	{
		g_utils.exit_status = EXIT_FAILURE;
		return (ERROR);
	}
	if (create_array(current) == ERROR)
		return (ERROR);
	return (1);
}
