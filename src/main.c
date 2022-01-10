/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 20:47:32 by fbindere          #+#    #+#             */
/*   Updated: 2022/01/10 20:57:12 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	check_empty_input(char *input)
{
	int	i;

	i = 0;
	while (check_whitespace(input[i]))
		i++;
	if (input[i] == '\0')
		return (1);
	else
		return (0);
}

int	get_next_line(char **line, int fd)
{
	char	buffer;
	int		flag;

	if(line == NULL)
		return(-1);
	*line = malloc(1);
	if(*line == NULL)
		return(-1);
	*line[0] = '\0';
	while ((flag = read(fd, &buffer, 1) > 0))
	{
		if(buffer == '\n')
			break;
		*line = ft_append(*line, buffer);
	}
	return(flag);		
}

static void	ghosthell(t_node **head)
{
	char	*read;

	//print_ghostshell();
	while (1)
	{
		signal_handler();
		
		read = readline("\e[1m	\033[1;34m༼ つ ❍_❍ ༽つ\033[0m\e[0m	");
		// if (get_next_line(&read, STDIN_FILENO) == 0)
		// 	read = NULL;
		clear_signals();
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		if (read != NULL && !ft_strcmp(read, ""))
		{
			add_history(read);
			if (!check_empty_input(read))
			{
				if (!lexer(head, read))
					executor(*head, 0, head);
			}
			free(read);
			free_nodes(head);
		}
		else if (read == NULL)
			ft_exit(EXIT_SUCCESS, head);
	}
}

int	main(__attribute__((unused)) int argc,
	__attribute__((unused)) char **argv, char **environ)
{
	t_node	*head;

	ft_copy_env(environ, -1, &head);
	head = NULL;
	ghosthell(&head);
	return (0);
}
