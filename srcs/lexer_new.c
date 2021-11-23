/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_new.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 15:28:10 by fbindere          #+#    #+#             */
/*   Updated: 2021/11/23 00:59:06 by eozben              ###   ########.fr       */
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
		if (*var_value == '*')
			*var_value = -42;
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
	variable[TMP] = ft_substr(*input, 0, i);
	if (!variable[TMP])
		return (free_toks(&new));
	*input += i;
	variable[VAR_VALUE] = getenv(variable[TMP]);
	free(variable[TMP]);
	variable[TMP] = NULL;
	if (state == DQUOTED_STATE && variable[VAR_VALUE] != NULL)
	{
		variable[TMP] = new->data;
		new->data = ft_strjoin(new->data, variable[VAR_VALUE]);
		if (!new->data)
			return (free_toks(&new));
		free(variable[TMP]);
	}
	else if (state == GENERAL_STATE && variable[VAR_VALUE] != NULL)
		return (get_variable_word(new, variable[VAR_VALUE]));
	return (0);
}

int	check_expansion(char **input, int *state, t_tok *new)
{
	int	ret;

	ret = check_state(**input, state);
	*input += ret;
	if (ret == CONTINUE)
		return (CONTINUE);
	if (*state == GENERAL_STATE && **input == '*')
	{
		new->data = ft_append(new->data, -42);
		while (**input == '*')
			*input += 1;
	}
	if (*state != SQUOTED_STATE && **input == '$')
	{
		if (expand_variable(*state, input, new) == -1)
			return (-1);
		return (CONTINUE);
	}
	return (0);
}

int	get_word(char **input, t_tok *new, int *state)
{
	int	ret;

	while (**input != '\0')
	{
		ret = check_expansion(input, state, new);
		if (ret == CONTINUE)
			continue ;
		ret = check_ctrlop_whitespace(*state, input);
		if (ret == NEW_TOK)
			break ;
		else if (ret == NEW_NODE)
			return (NEW_NODE);
		new->data = ft_append(new->data, **input);
		if (!new->data)
			return (free_toks(&new));
		*input += 1;
	}
	return (0);
}

int	handle_wildcards(t_tok *new)
{
	char			*wildcard;
	struct	dirent	*readdir;

	wildcard = ft_strchr(new->data, -42);
	if (wildcard == NULL)
		return (0);
	readdir = readdir()
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
		ret = get_word(input, new, &state);
		handle_wildcards(new);
		if (ret == NEW_NODE)
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