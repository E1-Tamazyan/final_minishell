/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elen_t13 <elen_t13@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:38:07 by elen_t13          #+#    #+#             */
/*   Updated: 2025/01/28 18:38:28 by elen_t13         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_heredoc_syntax(t_token *head)
{
	while (head)
	{
		if(head->type == 5)
		{
			head = head->next;
			if (head->type == 1 || head->type == 2 \
			|| head->type == 3 || head->type == 4 \
			|| head->type == 5)
			{
				printf("\nsyntax error unexpected token %s\n", head->context);
				set_exit_status(2);
				exit(get_exit_status()); // waiting for Alla's exit status
			}
		}
		head = head->next;
	}
}

void	check_heredoc_limit(t_shell *general)
{
	t_token	*head;
	int	count;

	count = 0;
	head = general->tok_lst;
	while (head)
	{
		if (head->type == REDIR_HEREDOC)
			count++;
		head = head->next;
	}
	if (count > 16)
	{
		printf("minisHell: maximum here-document count exceeded\n"); // check later pleaseee. SIGSEGV
		// waiting for Alla's cleaning function, general for this
		exit(2);
	}
	head = general->tok_lst;
	check_heredoc_syntax(head);
}