/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 15:28:10 by fbindere          #+#    #+#             */
/*   Updated: 2021/11/20 21:03:13 by eozben           ###   ########.fr       */
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
		return (WORD);
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

void	read_word(char **input, t_tok *token)
{
	int	state;
	int	ret;

	state = GENERAL_STATE;
	token->data = NULL;
	while (**input != '\0')
	{
		ret = check_state(**input, &state);
		*input += ret;
		if (ret == 1)
			continue ;
		if (check_type(*input) != WORD && state == GENERAL_STATE)
			break ;
		token->data = ft_append(token->data, **input);
		*input += 1;
	}
}

char	*ft_append(char *line, char c)
{
	int		length;
	int		i;
	char	*longer;

	if (line == NULL)
		line = ft_strdup("");
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

t_tok	*read_toks(t_tok **head, char *input)
{
	t_tok	*new;

	while (*input != '\0')
	{
		while (check_whitespace(*input))
			input++;
		new = ft_dll_append_tok(head);
		new->type = check_type(input);
		if (new->type > 127)
			input += 2;
		else if (new->type != WORD)
			input++;
		else
			read_word(&input, new);
	}
	return (new);
}

char	**expand_array(char **strarray)
{
	int		i;
	char	**new;

	i = 0;
	while (strarray[i] != NULL)
		i++;
	new = ft_calloc(sizeof(char *), i + 2);
	if (!new)
		return (NULL);
	i = 0;
	while (new[i] != NULL)
	{
		new[i] = strarray[i];
		i++;
	}
	free(strarray);
	return (new);
}