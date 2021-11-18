/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 20:31:46 by eozben            #+#    #+#             */
/*   Updated: 2021/11/18 20:47:16 by fbindere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_list(t_tok *head)
{
	while (head != NULL)
	{
		printf("%d\n", head->type);
		head = head->next;
	}
}

void	skip_whitespace(char *input)
{
	while (*input == ' ' || *input == '\n' || *input == '\t')
		input++;
}

t_token	check_type(char *c)
{
	if (c[0] == SQUOTE || c[0] == DQUOTE || c[0] == LPAREN	|| c[0] == RPAREN)
		return (c[0]);
	else if (c[0] == PIPE || c[0] == AMPERSAND || c[0] == GREAT || c[0] == LESS)
	{
		if (c[0] == c[1])
			return (c[0] * 4);
		return(c[0]);
	}
	else
		return (WORD);
}

int		handle_quotes(char *input, t_tok *current)
{
	int state[2];
	int	len;

	while (*input != '\0')
	{
		if (*input == SQUOTE)
			state[SQUOTE] = ON;
		else if (*input == DQUOTE)
			state[DQUOTE] = ON;
		while (state[SQUOTE] == ON || state[DQUOTE] == ON)
		{
			len++;
			input++;
			if (*input == SQUOTE)
				state[SQUOTE] = OFF;
			else if (*input == DQUOTE)
				state[DQUOTE] = OFF;
		}
	}
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
		else if (new->type != WORD)
			input++;
		else if (new->type == SQUOTE || new->type == DQUOTE)
			input += handle_quotes(input, new);
		else
			input += read_word(input, new->data);
		
	}
