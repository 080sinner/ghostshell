/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 15:28:10 by fbindere          #+#    #+#             */
/*   Updated: 2021/11/18 20:17:22 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	check_type(char *s)
{
	if (s[0] == LPAREN	|| s[0] == RPAREN || s[0] == SPACE || s[0] == TAB ||
		s[0] == NEWLINE)
		return (s[0]);
	else if (s[0] == PIPE || s[0] == AMPERSAND || s[0] == GREAT || s[0] == LESS)
	{
		if (s[0] == s[1])
			return (s[0] * 4);
		return(s[0]);
	}
	else
		return (WORD);
}

int	check_state(char c, int *state)
{
	if(c == SQUOTE && (*state == GENERAL_STATE || *state == SQUOTED_STATE))
	{
		if(*state == GENERAL_STATE)
			*state == SQUOTED_STATE;
		else if(*state == SQUOTED_STATE)
			*state == GENERAL_STATE;
		return (1);
	}
	else if(c == DQUOTE && (*state == GENERAL_STATE || *state == DQUOTED_STATE)
	{
		if(*state == GENERAL_STATE)
			*state == DQUOTED_STATE;
		else if(*state == DQUOTED_STATE)
			*state == GENERAL_STATE;
		return(1);
	}
	return(0);
}

void read_word(char *input, char *data)
{
	int state;

	state = GENERAL_STATE;
	data = ft_calloc(1, 1);
	if (data == NULL)
		return ;
	while(*input != NULL)
	{
		if(check_type(*input) != WORD && state == GENERAL_STATE)
			break;
		if(*input == SQUOTE || *input == DQUOTE)
			input += check_state(*input, &state);
		ft_append(data, *input);
		input++;
	}
	return(ft_strlen(data));
}

char *ft_append(char *line, char c)
{
	int i;
	char *longer;
	
	i = 0;
	while(line[i] != '\0')
		i++;
	longer = ft_calloc(i + 2);
	if(longer == NULL)
		return(NULL);
	i = 0;
	while(line[i] != '\0')
	{
		longer[i] = line[i];
		i++;
	}
	longer[i] = c;
	free(line);
	return(longer);
}


void	read_toks(t_tok *head, char *input)
{
	t_tok *new;

	while (*input != '\0')
	{
		skip_whitespace(input);
		new = ft_dll_append_tok(&head);
		new->type = check_type(input);
		if (new->type > 127)
			input += 2;
		else if (head->type != WORD)
			input++;
		else
			input += read_word(&input, new->data);
	}
}
	