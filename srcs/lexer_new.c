/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_new.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 15:28:10 by fbindere          #+#    #+#             */
/*   Updated: 2021/12/07 17:02:23 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_append(char *line, char c, t_node **head)
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
		exit(free_nodes(head));
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

int	check_expansion(char **input, int *state)
{
	if (*state == GENERAL_STATE && **input == '*')
		**input = -42;
	if (*state != SQUOTED_STATE && **input == '$')
	{
		if (check_whitespace(*(*input + 1)) || *(*input + 1) == '\0'
			|| is_control_op(*(*input + 1)) || !ft_isalnum(*(*input + 1)))
			return (0);
		**input = *state;
		return(1);
	}
	return (0);
}

void	mark_variable(char **input, t_tok *new, t_node **head)
{
	new->data = ft_append(new->data, **input, head);
	*input += 1;
	while (**input != '\0' && (ft_isalnum(**input) || **input== '_'))
	{
		new->data = ft_append(new->data, **input, head);
		*input += 1;
	}
	if(**input != '\0' && *(*input) == '?')
	{
		new->data = ft_append(new->data, **input, head);
		*input += 1;
	}
	new->data = ft_append(new->data, -4, head);
}

int	get_word(char **input, t_tok *new, int *state, t_node **head)
{
	while (**input != '\0')
	{
		if (check_state(input, state, new))
			continue ;
		if(check_expansion(input, state))
		{
			mark_variable(input, new, head);
			continue ;
		}
		if (*state == GENERAL_STATE && (check_whitespace(**input)
			|| is_control_op(check_type(*input)) == TRUE))
		{
			while (check_whitespace(**input))
				*input += 1;
			if (is_control_op(check_type(*input)) == TRUE)
				return (NEW_NODE);
			break;
		}
		new->data = ft_append(new->data, **input, head);
		*input += 1;
	}
	return (0);
}

int	read_command(char **input, t_node **command, t_node **head)
{
	int		state;
	t_tok	*new;

	state = GENERAL_STATE;
	new = NULL;
	(*command)->args = NULL;
	while (**input != '\0')
	{
		new = ft_dll_append_tok(&(*command)->args, head);
		new->data = ft_strdup("");
		if (!new)
			exit(free_nodes(head));
		if(get_word(input, new, &state, head) == NEW_NODE)
			return (NEW_NODE);
	}
	return (0);
}

int	read_input(t_node **head, char *input)
{
	t_node	*new;

	new = NULL;
	while (*input != '\0')
	{
		while (check_whitespace(*input))
			input++;
		new = ft_dll_append_node(head);
		new->type = check_type(input);
		if (new->type > 127)
			input += 2;
		else if (new->type != COMMAND)
			input++;
		else
			read_command(&input, &new, head);
	}
	return (0);
}

static void expand_node(t_node *node, t_node **head)
{
	t_tok	*current;
	t_tok	*newlist;

	current = node->args;
	while (current)
	{
		if(ft_strchr(current->data, END))
		{
			newlist = expand_variable(current->data, head, NULL, 0);
			insert_sublist(current, newlist);
			if(current->data)
				free (current->data);
			free(detach_tok(&node->args, current));
			current = newlist;
		}
		handle_wildcards(&current, &node->args, head);
		current = current->next;
	}
}

int	lexer(t_node **head, char *input)
{
	t_node	*tmp;

	read_input(head, input);
	tmp = *head;
	while (tmp != NULL)
	{
		expand_node(tmp, head);
		tmp = tmp->next;
	}
	return (0);
}

