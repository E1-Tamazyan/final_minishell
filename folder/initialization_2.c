/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elen_t13 <elen_t13@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 18:39:32 by elen_t13          #+#    #+#             */
/*   Updated: 2025/01/28 18:39:43 by elen_t13         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_env(char **env, t_shell *general)
{
	char	**sorted;

	general -> env_lst = init_env_nodes(env);
	sorted = sort_env(env);
	general -> sorted_env_lst = init_env_nodes(sorted);
	free_array(sorted);
	sorted = NULL;
	return (0);
}

t_cmd_lst	*initialize_new_cmd()
{
	t_cmd_lst	*new_cmd;

	new_cmd = (t_cmd_lst	*)malloc(sizeof(t_cmd_lst));
	check_malloc(new_cmd);
	new_cmd->cmd = NULL;
	new_cmd->args = NULL;
	new_cmd->next = NULL;
	new_cmd->heredoc = NULL;
	new_cmd->red_in = NULL;
	new_cmd->red_out = NULL;
	new_cmd->red_append = NULL;
	new_cmd->std_in = -2;
	new_cmd->std_out = -2;
	return (new_cmd);
}