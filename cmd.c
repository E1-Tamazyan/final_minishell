/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: algaboya <algaboya@student.42yerevan.am    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 21:57:54 by algaboya          #+#    #+#             */
/*   Updated: 2025/01/27 00:30:11 by algaboya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_commands(t_token *token)
{
	t_token	*temp;
	int		num;

	num = 1;
	temp = token;
	while (temp)
	{
		if (temp->type == 1)
			num++;
		temp = temp->next;
	}
	return (num);
}

void fill_commands(t_shell *general)
{
	t_token	*temp;
	int count;

	count = 0;
	if (general->curr_tok->type == WORD)
		general->curr_cmd->cmd = ft_strdup(general->curr_tok->context);
	temp = general->curr_tok;
	while (temp && temp->type == WORD && ++count)
		temp = temp->next;
	// printf("====>%d<======\n", count);
	if (count > 0)
	{
		general->curr_cmd->args = (char **)malloc(sizeof(char *) * (count + 1));
		if (!general->curr_cmd->args)
			return ;
		general->curr_cmd->args[count] = NULL;
	}	
}

int check_fill_commands(t_shell *g, int i, int j)
{
	printf("check_fill_commands\n");
	while (g->curr_tok && i < g->pipe_count + 1)
	{
		if (g->curr_tok->type == PIPE)
		{
			g->curr_tok = g->curr_tok->next;
			continue ;
		}
		fill_commands(g);
		j = 0;
		while (g->curr_tok && g->curr_tok->type == WORD)
		{
			g->curr_cmd->args[j] = ft_strdup(g->curr_tok->context);
			g->curr_tok = g->curr_tok ->next;
			j++;
		}
		if (g->curr_tok)
			g->curr_tok = g->curr_tok->next;
		if (g->curr_cmd->next)
			g->curr_cmd = g->curr_cmd->next;
		else
			g->curr_cmd->next = NULL;
		i++;
	}
	printf("end_check_fill_commands\n");
	return (EXIT_SUCCESS);
}
int create_cmd_lst(t_shell *g) // curr_cmd printable
{
	t_cmd_lst	*new;
	int			i;


	// printf("create_cmd_lst\n");
	g->pipe_count = count_commands(g->tok_lst) - 1;
	i = 0;
	g->cmd_lst = initialize_new_cmd();
	while (i < g->pipe_count)
	{
		new = initialize_new_cmd();
		check_malloc(g->cmd_lst);
		list_add_back_cmd(&g->cmd_lst, new);
		i++;
	}
	redirs_management(g);
	g->curr_cmd = g->cmd_lst;
	g->curr_tok = g->tok_lst;
	printf("before checking currr tok\n");
	// if (!g->curr_tok)
	// 	return (EXIT_SUCCESS); // Exit_success
	printf("after checking currr tok\n");
	return (check_fill_commands(g, 0, 0));
}
