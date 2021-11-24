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
		**input = -42;
		return (CONTINUE);
	}
	if (*state != SQUOTED_STATE && **input == '$')
	{
		if (expand_variable(*state, input, new) == -1)
			return (-1);
		return (CONTINUE);
	}
	return (0);
}

static int  free_array(char **split)
{
    int i;
    i = 0;
    while (split[i] != NULL)
        free(split[i++]);
    free(split);
    return (0);
}

static int  match_wildcard(char *filename, char *data)
{
    char    **split;
    int     i;
    split = ft_split(data, -42);
    if (split == NULL)
        return (0);
    i = 0;
    if (*split == NULL)
        return (1);
    while (split[i] != NULL)
    {
        if (data[0] != -42 && i == 0)
        {
            if (ft_strncmp(split[i], filename, ft_strlen(split[i])))
                return (free_array(split));
            filename += ft_strlen(split[i]);
            i++;
            continue ;
        }
        filename = ft_strnstr(filename, split[i], ft_strlen(filename));
        if (filename == NULL)
            return (free_array(split));
        filename += ft_strlen(split[i]);
        i++;
    }
    free_array(split);
    if (*filename == '\0' || *filename == -42)
        return (1);
    return (0);
}

t_tok   *handle_error_wildcard(t_tok **token, DIR *dir)
{
    free_toks(token);
    closedir(dir);
    return (NULL);
}

static int wildcard_expansion(t_tok **token, int checkvalue)
{
    DIR             *dir;
    t_tok           *new;
    struct dirent   *entity;
    dir = opendir(".");
    while (1)
    {
        entity = readdir(dir);
        if (entity == NULL)
            break ;
        if (entity->d_name[0] == '.')
            continue ;
        if (!match_wildcard(entity->d_name, (*token)->data))
            continue ;
        new = ft_dll_append_tok(token);
		if (!new)
			return (0);
		new->data = ft_strdup(entity->d_name);
		if (!new->data)
			return (0);
		checkvalue = 1;
	}
	closedir(dir);
	return (checkvalue);
}

int	handle_wildcards(t_tok **new)
{
	if (!ft_strchr((*new)->data, -42))
		return (0);
	if	(!wildcard_expansion(new, 0))
	{
		while (ft_strchr((*new)->data, -42))
			*(ft_strchr((*new)->data, -42)) = '*';
	}
	else
		free(detach_tok(new, *new));
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
		handle_wildcards(&new);
		if (ret == NEW_NODE)
			return (NEW_NODE);
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