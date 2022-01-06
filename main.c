/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eozben <eozben@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 20:47:32 by fbindere          #+#    #+#             */
/*   Updated: 2022/01/06 00:57:33 by eozben           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	print_list(t_node *head)
{
	t_node	*tmpnode;
	t_tok	*tmptok;

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
				printf("'%s' ", tmptok->data);
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
			while (tmptok)
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

void	ft_copy_env(char **environ, int skip_var, t_node **head)
{
	int			i;
	int			x;

	i = 0;
	while (environ[i])
		i++;
	g_utils.environment = ft_calloc(i + 2, sizeof(char *));
	if (!g_utils.environment)
		ft_exit(EXIT_FAILURE, head);
	i = 0;
	x = i;
	while (environ[i])
	{
		if (skip_var == -1 || x != skip_var)
		{
			g_utils.environment[i] = ft_strdup(environ[x]);
			i++;
		}
		x++;
	}
}

void	get_input(t_node **head)
{
	char	*read;

	//print_ghostshell();
	while (1)
	{
		signal_handler();
		read = readline("\e[1m	\033[1;34m༼ つ ❍_❍ ༽つ\033[0m\e[0m	");
		clear_signals();
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		if (read != NULL && !ft_strcmp(read, ""))
		{
			add_history(read);
			if (!check_empty_input(read))
			{
				if (!lexer(head, read))
					executor(*head, head);
				//print_list(*head);
			}
			free(read);
			free_nodes(head);
		}
		else if (read == NULL)
		{
			free_nodes(head);
			ft_exit(EXIT_SUCCESS, head);
		}
	}
}


int	main(__unused int argc, __unused char *argv[], char **environ)
{
	t_node	*head;

	ft_copy_env(environ, -1, &head);
	head = NULL;
	get_input(&head);
	// free_nodes(&head);
	head = NULL;
	ft_free_strarray(g_utils.environment);
	system("leaks minishell");
	return (0);
}