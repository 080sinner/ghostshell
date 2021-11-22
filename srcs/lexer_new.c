/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_new.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 15:28:10 by fbindere          #+#    #+#             */
/*   Updated: 2021/11/22 20:25:54 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	get_variable_word(t_tok *current, char *var_value)
{
	t_tok	*new;

	new = current;
	while (*var_value != '\0')
	{
		if (check_whitespace(*var_value))
		{
			new = ft_dll_append_tok(&new);
			if (!new)
				return (free_toks(&new));
			new->data = ft_strdup("");
			if (!new)
				return (free_toks(&new));
		}
		new->data = ft_append(new->data, *var_value);
		if (!new->data)
			return (free_toks(&new));
		var_value++;
	}
	return (0);
}

int	expand_variable(int state, char **input, t_tok *new)
{
	int		i;
	char	*variable[2];

	*input += 1;
	i = 0;
	while (ft_isalnum((*input)[i]) || (*input)[i] == '_')
		i++;
	variable[1] = ft_substr(*input, 0, i);
	if (!variable[1])
		return (free_toks(&new));
	*input += i;
	variable[0] = getenv(variable[1]);
	free(variable[1]);
	variable[1] = NULL;
	if (state == DQUOTED_STATE && variable[1] != NULL)
	{
		variable[0] = new->data;
		new->data = ft_strjoin(new->data, variable[1]);
		if (!new->data)
			return (free_toks(&new));
		free(variable[0]);
	}
	else if (state == GENERAL_STATE && variable[1] != NULL)
		return (get_variable_word(new, variable[1]));
	return (0);
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
		if (state != SQUOTED_STATE && **input == '$')
		{
			if (expand_variable(state, input, new) == -1)
				return (-1);
			continue ;
		}
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