/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 20:31:46 by eozben            #+#    #+#             */
/*   Updated: 2021/11/16 19:18:15 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// void	ignore_signals(void)
// {
// 	signal(SIGINT, SIG_IGN);
// 	signal(SIGTSTP, SIG_IGN);
// 	signal(SIGQUIT, SIG_IGN);
// }

int	main(void)
{
	char	*read;

	while (1)
	{
		read = readline("minish $ ");
		if (!ft_strcmp(read, "\0"))
			add_history(read);
		free(read);
		if (ft_strcmp(read, "exit"))
			break ;
	}
	return (0);
}
