/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_control_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/15 19:45:08 by fbindere          #+#    #+#             */
/*   Updated: 2022/01/15 19:46:44 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	print_error_type(t_node *node, t_tok *token)
{
	if (node && node->type)
	{
		if (node->type == PIPE)
			return (ft_printnl_fd(" '|' ", 2));
		if (node->type == AND)
			return (ft_printnl_fd(" '&&' ", 2));
		if (node->type == OR)
			return (ft_printnl_fd(" '||' ", 2));
	}
	else if (token && token->type)
	{
		if (token->type == GREAT)
			return (ft_printnl_fd(" > ", 2));
		if (token->type == GREATGREAT)
			return (ft_printnl_fd(" >> ", 2));
		if (token->type == LESS)
			return (ft_printnl_fd(" < ", 2));
		if (token->type == LESSLESS)
			return (ft_printnl_fd(" << ", 2));
	}
	return (1);
}

t_node	*search_lparen(t_node *current, int first_call)
{
	static int	parencount;

	if (first_call == 0)
		parencount = 0;
	if (!current)
		return (NULL);
	if (current->type == RPAREN)
		parencount++;
	if (current->type == LPAREN)
		parencount--;
	if (parencount == 0)
		return (current);
	return (search_lparen(current->previous, ++first_call));
}
