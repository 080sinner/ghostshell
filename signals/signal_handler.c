/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 21:34:56 by fbindere          #+#    #+#             */
/*   Updated: 2022/01/06 00:10:33 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	clear_signals(void)
{
	struct termios	term;

	tcgetattr(1, &term);
	if ((term.c_lflag & (0x1 << 6)) != ECHOCTL)
	{
		term.c_lflag += ECHOCTL;
		tcsetattr(1, 0, &term);
	}
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
	if ((term.c_lflag & (0x1 << 6)) == ECHOCTL)
	{
		term.c_lflag -= ECHOCTL;
		tcsetattr(1, 0, &term);
	}
	return (0);
}
