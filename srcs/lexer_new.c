/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_new.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 15:28:10 by fbindere          #+#    #+#             */
/*   Updated: 2022/01/04 23:42:32 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_append(char *line, char c)
{
	int		length;
	int		i;
	char	*longer;

	if (line == NULL)
		return (NULL);
	length = ft_strlen(line);
	longer = ft_calloc(length + 2, sizeof(char));
	if (longer == NULL)
		ft_free((void *)&line, ft_strlen(line));
	i = 0;
	while (line && line[i] != '\0')
	{
		longer[i] = line[i];
		i++;
	}
	if (longer)
		longer[i] = c;
	if (line)
		free(line);
	return (longer);
}

int	check_expansion(char **input, int *state)
{
	if (*state == GENERAL_STATE && **input == '*')
		**input = -42;
	if (*state != SQUOTED_STATE && **input == '$')
	{
		if ((!ft_isalnum(*(*input + 1)) && *(*input + 1) != '_' && *(*input + 1) != '?')
			|| (check_whitespace(*(*input + 1)) || *(*input + 1) == '\0'
				|| is_control_op(*(*input + 1))))
			return (0);
		**input = *state;
		return (1);
	}
	return (0);
}

int		mark_variable(char **input, t_tok *new)
{
	new->data = ft_append(new->data, **input);
	if (!new->data)
		return (-1);
	*input += 1;
	while (**input != '\0' && (ft_isalnum(**input) || **input== '_'))
	{
		new->data = ft_append(new->data, **input);
		if (!new->data)
			return (-1);
		*input += 1;
	}
	if(**input != '\0' && *(*input) == '?')
	{
		new->data = ft_append(new->data, **input);
		if (!new->data)
			return (-1);
		*input += 1;
	}
	new->data = ft_append(new->data, -4);
	if (!new->data)
		return (-1);
	return (0);
}

int	get_word(char **input, t_tok *new, int *state)
{
	while (**input != '\0')
	{
		if (check_state(input, state, new))
			continue ;
		if(check_expansion(input, state))
		{
			if (mark_variable(input, new) == -1)
				return (-1);
			continue ;
		}
		if (*state == GENERAL_STATE && (check_whitespace(**input)
			|| is_control_op(check_type(*input)) || is_redir_op(*input)))
		{
			while (check_whitespace(**input))
				*input += 1;
			if (is_control_op(check_type(*input)) == TRUE)
				return (NEW_NODE);
			break;
		}
		new->data = ft_append(new->data, **input);
		if (!new->data)
			return (-1);
		*input += 1;
	}
	return (0);
}

int	read_command(char **input, t_node **command, int *state)
{
	t_tok	*new;
	int		ret;

	*state = GENERAL_STATE;
	new = NULL;
	(*command)->args = NULL;
	while (**input != '\0')
	{
		new = create_new_tok(&(*command)->args);
		if (!new)
			return (-1);
		if (is_redir_op(*input))
		{
			new->type = is_redir_op(*input);
			*input += 1;
			if (new->type > 127)
				*input += 1;
			while (check_whitespace(**input))
				*input += 1;
			continue ;
		}
		ret = get_word(input, new, state);
		if (ret == -1)
			return (-1);
		else if (ret == NEW_NODE)
			return (NEW_NODE);
	}
	return (0);
}

int	read_input(t_node **head, char *input)
{
	t_node	*new;
	int		state;

	state = 0;
	new = NULL;
	while (*input != '\0')
	{
		while (check_whitespace(*input))
			input++;
		new = ft_dll_append_node(head);
		if (!new)
			return (-1);
		new->type = check_type(input);
		if (new->type > 127)
			input += 2;
		else if (new->type != COMMAND)
			input++;
		else if (read_command(&input, &new, &state) == -1)
			return (-1);
	}
	return (state);
}

void	free_tok(t_tok **head, t_tok *tok)
{
	if (tok->data)
		ft_free((void *)&tok->data, ft_strlen(tok->data));
	free(detach_tok(head, tok));
}

int	expander(t_node *node)
{
	t_tok	*current;
	t_tok	*newlist;
	t_tok	*tmp;

	current = node->args;
	while (current)
	{
		if (ft_strchr(current->data, END))
		{
			newlist = expand_variable(current->data, NULL, 0);
			if (!newlist)
			{
				free_tok(&node->args, current);
				return (-1);
			}
			insert_sublist(current, newlist);
			free_tok(&node->args, current);
			current = newlist;
		}
		if (node->here_doc && node->here_doc->state == FALSE)
		{
			if (expand_here_doc(node->here_doc) == -1)
				return (-1);
		}
		tmp = current->next;
		if (expand_wildcards(&current, &node->args) == -1)
			return (-1);
		current = tmp;
	}
	return (0);
}

int	lexer(t_node **head, char *input)
{
	int		ret;

	ret = 0;
	ret = read_input(head, input);
	if (ret == -1)
		return (-1);
	else if (ret != GENERAL_STATE)
		return (printf("syntax error: unequal amount of quotes\n"));
	if (check_input(head))
		return (1);
	if (read_here_docs(head) == -1)
		return (-1);
	return (0);
}
