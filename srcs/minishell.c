/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbindere <fbindere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 20:31:46 by eozben            #+#    #+#             */
/*   Updated: 2021/11/18 16:29:25 by fbindere         ###   ########.fr       */
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
	if (c[0] == LPAREN	|| c[0] == RPAREN)
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
			input += read_word(input, new->data);
		
	}

	







int	main(void)
{
	char	*read;
	t_tok	*head;

	head = NULL;
	while (1)
	{
		read = readline("minish $ ");
		if (!ft_strcmp(read, "\0"))
			add_history(read);
		read_toks(head, read);
		free(read);
		if (ft_strcmp(read, "exit"))
			break ;
	}
	check_meta(' ');
	return (0);
}
