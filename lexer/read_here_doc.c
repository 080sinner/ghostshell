/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_here_doc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 20:57:30 by fbindere          #+#    #+#             */
/*   Updated: 2022/01/17 16:13:15 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*convert_variable_delimiter(char *data)
{
	char	*new_var;
	int		counter[2];

	new_var = ft_calloc(ft_strlen(data), sizeof(char));
	if (!new_var)
		return (ft_free((void *)&data, ft_strlen(data)));
	while (data && ft_strchr(data, GENERAL_STATE))
		*(ft_strchr(data, GENERAL_STATE)) = '$';
	while (data && ft_strchr(data, DQUOTED_STATE))
		*(ft_strchr(data, DQUOTED_STATE)) = '$';
	counter[0] = 0;
	counter[1] = 0;
	while (data[counter[0]])
	{
		if (data[counter[0]] != END)
		{
			new_var[counter[1]] = data[counter[0]];
			counter[1]++;
		}
		counter[0]++;
	}
	ft_free((void *)&data, ft_strlen(data));
	return (new_var);
}

static char	*read_heredoc_prompt(void)
{
	char			*str;
	char			*tmp;
	struct termios	term;

	tcgetattr(1, &term);
	term.c_lflag &= ~ECHOCTL;
	write(1, ">", 1);
	signal(SIGINT, signal_heredoc);
	tcsetattr(1, 0, &term);
	str = get_next_line(0);
	tmp = str;
	str = ft_substr(tmp, 0, ft_strlen(tmp) - 1);
	free(tmp);
	term.c_lflag |= ECHOCTL;
	tcsetattr(1, 0, &term);
	return (str);
}

static int	read_here_doc(t_tok *delim, t_node *command)
{
	char	*line;
	t_tok	*new;

	line = read_heredoc_prompt();
	if (!line || ft_strcmp(line, delim->data) || delim->type != COMMAND)
	{
		if (line)
			free(line);
		return (0);
	}
	new = ft_dll_append_tok(&(command->here_doc));
	if (!new)
		return (ERROR);
	new->data = line;
	new->state = delim->state;
	return (1);
}

static int	here_doc(t_node *command, t_tok *delim)
{
	int		tmp_fd;
	int		ret;

	if (!delim)
		return (0);
	if (delim->data && ft_strchr(delim->data, END))
		delim->data = convert_variable_delimiter(delim->data);
	if (!delim->data)
		return (ERROR);
	tmp_fd = dup(STDIN_FILENO);
	while (1)
	{
		ret = read_here_doc(delim, command);
		if (ret == ERROR)
			return (ERROR);
		else if (!ret)
			break ;
	}
	dup2(tmp_fd, STDIN_FILENO);
	close(tmp_fd);
	if (errno == 9)
		return (ERROR);
	return (0);
}

int	read_here_docs(t_node **head)
{
	t_node	*curr_node;
	t_tok	*curr_tok;

	curr_node = *head;
	while (curr_node)
	{
		curr_tok = curr_node->args;
		while (curr_tok)
		{
			if (curr_tok->type == LESSLESS)
			{
				if (curr_node->here_doc)
					free_toks(&(curr_node->here_doc));
				if (here_doc(curr_node, curr_tok->next) == ERROR)
					return (ERROR);
			}
			curr_tok = curr_tok->next;
		}
		curr_node = curr_node->next;
	}
	return (0);
}
