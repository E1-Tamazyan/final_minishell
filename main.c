/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elen_t13 <elen_t13@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 14:42:13 by etamazya          #+#    #+#             */
/*   Updated: 2025/01/28 18:05:03 by elen_t13         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// ************************
// *** WARNING *** FULL ***
// ************************

int	main(int argc, char **argv, char **env)
{
	(void)argv;
	t_shell *general;
	char	*input;

	input = NULL;
	general = (t_shell *)malloc(sizeof(t_shell));
	check_malloc(general); 
	if (argc == 1)
	{
		init_general(general);
		create_env(env, general);
		incr_shlvl(general);
		if (init_input(input, general)) // if 1 error
			return (free(general), get_exit_status());
	}
	free(general);
	return (get_exit_status());
}


// check leaks
// valgrind --leak-check=full ./your_program
//  valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./minishell