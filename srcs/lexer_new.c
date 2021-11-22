/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_new.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 15:28:10 by fbindere          #+#    #+#             */
/*   Updated: 2021/11/22 15:12:09 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*ft_append(char *line, char c)
{
	int		length;
	int		i;
	char	*longer;

	if (line == NULL)
		return (NULL);
	length = ft_strlen(line);
	longer = ft_calloc(length + 2, sizeof(char));
	if (longer == NULL)
	{
		free(line);
		return (NULL);
	}
	i = 0;
	while (line[i] != '\0')
	{
		longer[i] = line[i];
		i++;
	}
	longer[i] = c;
	free(line);
	return (longer);
}

int	get_word(char **input, t_tok *new, int state)
{
	int	ret;

	while (**input != '\0')
	{
		ret = check_state(**input, &state);
		*input += ret;
		if (ret == 1)
			continue ;
		ret = check_ctrlop_whitespace(state, input);
		if (ret == 2)
			break ;
		else if (ret == 1)
			return (1);
		new->data = ft_append(new->data, **input);
		if (!new->data)
			return (free_toks(&new));
		*input += 1;
	}
	return (0);
}

int	read_command(char **input, t_node *command)
{
	int		state;
	t_tok	*new;
	int		ret;

	state = GENERAL_STATE;
	new = NULL;
	command->args = NULL;
	while (**input != '\0')
	{
		new = ft_dll_append_tok(&command->args);
		if (!new)
			return (free_nodes(&command));
		new->data = ft_strdup("");
		if (!new)
			return (free_nodes(&command));
		ret = get_word(input, new, state);
		if (ret == 1)
			return (0);
		else if (ret == -1)
			return (free_nodes(&command));
	}
	return (0);
}

int	read_toks(t_node **head, char *input)
{
	t_node	*new;

	new = NULL;
	while (*input != '\0')
	{
		while (check_whitespace(*input))
			input++;
		new = ft_dll_append_node(head);
		if (!new)
			return (free_nodes(head));
		new->type = check_type(input);
		if (new->type > 127)
			input += 2;
		else if (new->type != COMMAND)
			input++;
		else
		{
			if (read_command(&input, new) == -1)
				return (free_nodes(head));
		}
	}
	return (0);
}

// char	**expand_array(char **strarray)
// {
// 	int		i;
// 	char	**new;

// 	i = 0;
// 	while (strarray[i] != NULL)
// 		i++;
// 	new = ft_calloc(sizeof(char *), i + 2);
// 	if (!new)
// 		return (NULL);
// 	i = 0;
// 	while (new[i] != NULL)
// 	{
// 		new[i] = strarray[i];
// 		i++;
// 	}
// 	free(strarray);
// 	return (new);
// }