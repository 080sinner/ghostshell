/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_new.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 15:28:10 by fbindere          #+#    #+#             */
/*   Updated: 2021/11/28 00:43:41 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_expansion(char **input, int *state, t_tok *new)
{
	int	ret;

	ret = check_state(**input, state, new);
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
		if (check_whitespace(*(*input + 1)) || *(*input + 1) == '\0'
			|| is_control_op(*(*input + 1)))
			return (0);
		ret = expand_variable(*state, input, new);
		if (ret == -1)
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

int	read_command(char **input, t_node **command)
{
	int		state;
	t_tok	*new;
	int		ret;

	state = GENERAL_STATE;
	new = NULL;
	(*command)->args = NULL;
	while (**input != '\0')
	{
		new = ft_dll_append_tok(&(*command)->args);
		if (!new)
			return (free_nodes(command));
		new->data = ft_strdup("");
		if (!new)
			return (free_nodes(command));
		ret = get_word(input, new, &state);
		if (!(is_redir_op((*command)->previous) && new->previous == NULL))
			handle_wildcards(&new, &(*command)->args);
		if (ret == NEW_NODE)
			return (NEW_NODE);
		else if (ret == -1)
			return (free_nodes(command));
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
			if (read_command(&input, &new) == -1)
				return (free_nodes(head));
		}
	}
	return (0);
}

int	lexer(t_node **head, char *input)
{
	t_node	*tmp;
	// t_tok	*tmp2;

	if (read_toks(head, input) == -1)
		return (-1);
	tmp = *head;
	while (tmp != NULL)
	{
		if (tmp->type == LESSLESS && tmp->next != NULL)
		{
			here_doc(tmp);
			// tmp2 = detach_tok(&tmp->args, tmp->args);
			// ft_free((void *)&tmp2->data, ft_strlen(tmp2->data));
			// free(tmp2);
		}
		tmp = tmp->next;
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