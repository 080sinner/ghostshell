/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mac <mac@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 21:34:56 by fbindere          #+#    #+#             */
/*   Updated: 2022/01/15 23:52:21 by mac              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	clear_signals(void)
{
	struct termios	term;

	tcgetattr(1, &term);
	term.c_lflag |= ECHOCTL;
	tcsetattr(1, 0, &term);
}

void	sig_ctrl(int signum)
{
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
	}
	rl_on_new_line();
	rl_redisplay();
}

int	signal_handler(void)
{
	struct termios	term;

	signal(SIGINT, sig_ctrl);
	signal(SIGQUIT, sig_ctrl);
	tcgetattr(1, &term);
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(1, 0, &term);
	return (0);
}
