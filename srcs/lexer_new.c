/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_new.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 15:28:10 by fbindere          #+#    #+#             */
/*   Updated: 2021/11/21 22:58:53 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	check_type(char *s)
{
	if (s[0] == LPAREN || s[0] == RPAREN || s[0] == SPACE || s[0] == TAB
		|| s[0] == NEWLINE)
		return (s[0]);
	else if (s[0] == PIPE || s[0] == AMPERSAND || s[0] == GREAT || s[0] == LESS)
	{
		if (s[0] == s[1])
			return (s[0] * 4);
		return (s[0]);
	}
	else
		return (COMMAND);
}

int	check_state(char c, int *state)
{
	if (c == SQUOTE && (*state == GENERAL_STATE || *state == SQUOTED_STATE))
	{
		if (*state == GENERAL_STATE)
			*state = SQUOTED_STATE;
		else if (*state == SQUOTED_STATE)
			*state = GENERAL_STATE;
		return (1);
	}
	if (c == DQUOTE && (*state == GENERAL_STATE || *state == DQUOTED_STATE))
	{
		if (*state == GENERAL_STATE)
			*state = DQUOTED_STATE;
		else if (*state == DQUOTED_STATE)
			*state = GENERAL_STATE;
		return (1);
	}
	return (0);
}

void	read_command(char **input, t_node *command)
{
	int	state;
	int	ret;
	t_tok *new;

	state = GENERAL_STATE;
	new = NULL;
	command->args = NULL;
	while (**input != '\0')
	{
		new = ft_dll_append_tok(&command->args);
		new->data = ft_strdup("");
		while(**input != '\0')
		{
			ret = check_state(**input, &state);
			*input += ret;
			if (ret == 1)
				continue ;
			if(state == GENERAL_STATE && (check_whitespace(**input)
				|| is_control_op(check_type(*input)) == TRUE))
			{
				while (check_whitespace(**input))
					*input += 1;
				if(is_control_op(check_type(*input)) == TRUE)
					return ;
				else
					break ;
			}
			new->data = ft_append(new->data, **input);
			*input += 1;
		}
	}
}

char	*ft_append(char *line, char c)
{
	int		length;
	int		i;
	char	*longer;

	if (line == NULL)
		return(NULL);
	length = ft_strlen(line);
	longer = ft_calloc(length + 2, sizeof(char));
	if (longer == NULL)
		return (NULL);
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

t_node	*read_toks(t_node **head, char *input)
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
			read_command(&input, new);
	}
	return (new);
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