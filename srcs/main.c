/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 20:47:32 by fbindere          #+#    #+#             */
/*   Updated: 2021/12/16 12:51:22 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_list(t_node *head)
{
	int		i;
	t_node	*tmpnode;
	t_tok	*tmptok;

	i = 0;
	tmpnode = head;
	while (tmpnode != NULL)
	{
		printf("		NEWNODE\n");
		if (tmpnode->type != COMMAND && tmpnode->type <= 127)
			printf("OPERATOR :%c", tmpnode->type);
		if (tmpnode->type != COMMAND && tmpnode->type == 152)
			printf("OPERATOR :&& ");
		if (tmpnode->type != COMMAND && tmpnode->type == 496)
			printf("OPERATOR :|| ");
		tmptok = tmpnode->args;
		if(tmpnode->args)
			printf("COMMAND: ");
		while (tmptok != NULL)
		{
			if (tmptok->type == COMMAND)
				printf("%s ", tmptok->data);
			else if (tmptok->type == LESS)
				printf("< ");
			else if (tmptok->type == LESSLESS)
				printf("<< ");
			else if (tmptok->type == GREAT)
				printf("> ");
			else if (tmptok->type == GREATGREAT)
				printf(">> ");
			tmptok = tmptok->next;
		}
		tmptok = tmpnode->here_doc;
		if (tmpnode->type == COMMAND && tmpnode->here_doc)
		{
			printf("\nHEREDOC: ");
			while(tmptok)
			{
				printf("%s ", tmptok->data);
				tmptok = tmptok->next;
			}
		}
		tmpnode = tmpnode->next;
		printf("\n");
	}
}

static int	check_empty_input(char *input)
{
	int	i;

	i = 0;
	while (check_whitespace(input[i]))
		i++;
	if (input[i] == '\0')
		return (1);
	else
		return (0);
}

void	get_input(t_node **head)
{
	char	*read;

	print_ghostshell();
	while (1)
	{
		read = readline("\e[1m	\033[1;34m༼ つ ❍_❍ ༽つ\033[0m\e[0m	");
		if (read != NULL && !ft_strcmp(read, ""))
		{
			add_history(read);
			if (ft_strcmp(read, "exit"))
			{
				free(read);
				break ;
			}
			if (!check_empty_input(read))
			{
				lexer(head, read);
				executor(NULL, NULL, NULL, NULL, head);
				print_list(*head);
			}
			free(read);
			free_nodes(head);
		}
	}
}


int	main(void)
{
	t_node	*head;

	head = NULL;
	get_input(&head);
	// free_nodes(&head);
	head = NULL;
	//system("leaks minishell");
	return (0);
}
