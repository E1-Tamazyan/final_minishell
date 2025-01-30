/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elen_t13 <elen_t13@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 19:38:08 by algaboya          #+#    #+#             */
/*   Updated: 2025/01/28 18:44:35 by elen_t13         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void init_general(t_shell *general)
{
	general->tok_lst = NULL;
	general->env_lst = NULL;
	general->cmd_lst = NULL;
	// general->doll_lst = NULL; // check this later
	general->doll_lst = (t_dollar *)malloc(sizeof(t_dollar)); // check this later
	general->sorted_env_lst = NULL;
	general->shlvl = -1;
	general->sg_quote = 0; //no quote
	general->db_quote = 0; // no quote
	general->pipe_index = 0;
	// general->exit_status = 0;
	general->pipe_count = 0;
}

int	init_input(char *input, t_shell *general)
{
	static int	index;

	while (1)
	{
		index = 0;
		init_signal(1);
		input = readline("\033[38;5;51m\033[48;5;16mminisHell:\033[0m "); //neon
		if(input && input[0])
		{
			add_history(input);
			if (init_tokens_cmds(input, general, 0) == 0)
			{
				printf("______________***\n");
				execution(general, index);
				printf("______________22***\n");
				// free_cmd_lst(&general->cmd_lst);
				printf("______________33***\n");
				// general->cmd_lst = NULL;
				close_pipes(general->fd, general->pipe_count);
				printf("______________44***\n");
			}
			free(input);
		}
		else if (!input)
		 	exit(get_exit_status());
		// else
		// 	break ;	
	}
	return (free(input), get_exit_status());
}

t_env *init_env_nodes(char **env)
{
	t_env	*list_env;
	t_env	*tmp;
	t_env	*new_node;
	int		i;

	i = 0;
	list_env = NULL;
	tmp = NULL;
	while (env[i] != NULL)
	{
		new_node = ft_lstnew(env[i], 1);
		if (!new_node) 
			return NULL;
		if (list_env == NULL)
		{
			list_env = new_node;
			tmp = list_env;
		}
		else
			ft_lstadd_back(tmp, new_node);
		i++;
	}
	return (list_env);
}

int	init_tokens_cmds(char *input, t_shell *general, int i)
{
	int	start;
	int flag;

	flag = 0;
	while ((input[i] >= 9 && input[i] <= 13) || input[i] == 32)
		i++;
	while (flag >= 0 && input[i] != '\0')
	{
		if (flag >= 0 && input[i] && (input[i] == '|' || input[i] == '>'
			|| input[i] == '<' || input[i] == ' '))
				flag = init_op_token(input, &i, &general->tok_lst);
		else
		{
			start = i;
			while (flag >= 0 && input[i] && input[i] != '|' && input[i] != '>' && input[i] != '<'
				&& input[i] != ' ' && input[i] != '$' && input[i] != 34 && input[i] != 39)
				i++;
			if (input[i] && flag >= 0)
				flag = check_cut_quotes(general, &input, &i, start); // and added dollar sign here check_cut_quotes
			else if (i > start)
				add_token_list(&general->tok_lst, my_substr((const char *)input, start, i - start), 0);
			i--;
		}
		if(flag < 0)
			return (clean_list(&general->tok_lst), -1);
		if (input[i])
			i++;
	}
		printf("******\n");
	print_tokens(general->tok_lst);
	printf("******\n");
	// printf("sss = %s\n", general->tok_lst->context);
	general->tok_lst = remove_extra_quotes(general);
	check_heredoc_limit(general);
	create_cmd_lst(general);
	printf("_____******______\n");
	print_cmd(general->cmd_lst);
	clean_list(&general->tok_lst);
	printf("_____******______\n");
// 	return (0);
	return (0);
}

// int init_op_token(char *input, int *i, t_token **token_list)
// {
// 	if (!input || !token_list)
// 		return -1;
// 	// Check for '|' character
// 	if (input[*i] && input[*i] == '|')
// 	{
// 		if (!input[*i + 1] || (input[*i + 1] != '|' && !input[*i + 2])) // Handle syntax error
// 			return (printf("minisHell: syntax error near unexpected token `newline'\n"), -1);
// 		if (input[*i + 1] == '|')
// 			return (printf("minisHell: syntax error near unexpected token `||'\n"), -1);
// 		add_token_list(token_list, my_substr(input, *i, 1), 1);
// 	}
// 	else if (input[*i] && input[*i] == '>')
// 	{
// 		// Handle '>' and '>>' tokens
// 		if (!input[*i + 1] || (input[*i + 1] != '<' && !input[*i + 2])) // Handle error
// 			return (printf("minisHell: syntax error near unexpected token `newline'\n"), -1);
// 		if (input[*i + 1] && input[*i + 1] == '>')
// 		{
// 			if (input[*i + 2] && (input[*i + 2] == '>' || input[*i + 2] == '<' || input[*i + 2] == '|'))
// 				return (printf("minisHell: syntax error near unexpected token `%c%c'\n", input[*i + 2], input[*i + 3]), -1);
// 			add_token_list(token_list, my_substr(input, *i, 2), 4);
// 			(*i)++;
// 		}
// 		else if (input[*i + 1] && input[*i + 1] == '<')
// 			return (printf("minisHell: syntax error near unexpected token `%c%c'\n", input[*i + 1], input[*i + 2]), -1); //Handle error for invalid combinations like '><' or '<|'
// 		else
// 			add_token_list(token_list, my_substr(input, *i, 1), 3);
// 	}
// 	else if (input[*i] && input[*i] == '<')
// 	{
// 		// Handle '<' and '<<' tokens
// 		if (!input[*i + 1] || (input[*i + 1] != '>' && !input[*i + 2])) // Handle error
// 			return (printf("minisHell: syntax error near unexpected token `newline'\n"), -1);
// 		if (input[*i + 1] && input[*i + 1] == '<')
// 		{
// 			if (input[*i + 2] && (input[*i + 2] == '>' || input[*i + 2] == '<'))
// 				return (printf("minisHell: syntax error near unexpected token `%c%c'\n", input[*i + 2], input[*i + 3]), -1);
// 			add_token_list(token_list, my_substr(input, *i, 2), 5);
// 			(*i)++;
// 		}
// 		else
// 			add_token_list(token_list, my_substr(input, *i, 1), 2);
// 	}
// 	return (*i);
// }


















static int check_dollar_sign(char *input, int *i, t_token **token_list)
{
	if (input[*i] && input[*i] == '$')
	{
		if (input[*i + 1] && input[*i + 1] == '$')
			add_token_list(token_list, my_substr(input, *i, 2), 4);
		(*i)++;
		return 1;
	}
	return 0;
}

static int check_pipe_sign(char *input, int *i, t_token **token_list)
{
	if (input[*i] && input[*i] == '|')
	{
		if (!input[*i + 1] || (input[*i + 1] != '|' && !input[*i + 2])) // Syntax error
			return (printf("minisHell: syntax error near unexpected token `newline'\n"), -1);
		if (input[*i + 1] == '|') // Handle `||` error
			return (printf("minisHell: syntax error near unexpected token `||'\n"), -1);
		add_token_list(token_list, my_substr(input, *i, 1), 1);
		return 1;
	}
	return 0;
}

static int check_greater_than_sign(char *input, int *i, t_token **token_list)
{
	if (input[*i] && input[*i] == '>')
	{
		if (!input[*i + 1] || (input[*i + 1] != '<' && !input[*i + 2])) // Syntax error
			return (printf("minisHell: syntax error near unexpected token `newline'\n"), -1);

		if (input[*i + 1] && input[*i + 1] == '>')
		{
			if (input[*i + 2] && (input[*i + 2] == '>' || input[*i + 2] == '<' || input[*i + 2] == '|'))
				return (printf("minisHell: syntax error near unexpected token `%c%c'\n", input[*i + 2], input[*i + 3]), -1);
			add_token_list(token_list, my_substr(input, *i, 2), 4);
			(*i)++;
		}
		else if (input[*i + 1] && input[*i + 1] == '<')
		{
			return (printf("minisHell: syntax error near unexpected token `%c%c'\n", input[*i + 1], input[*i + 2]), -1);
		}
		else
			add_token_list(token_list, my_substr(input, *i, 1), 3);
		return 1;
	}
	return 0;
}

static int check_less_than_sign(char *input, int *i, t_token **token_list)
{
	if (input[*i] && input[*i] == '<')
	{
		if (!input[*i + 1] || (input[*i + 1] != '>' && !input[*i + 2])) // Syntax error
			return (printf("minisHell: syntax error near unexpected token `newline'\n"), -1);

		if (input[*i + 1] && input[*i + 1] == '<')
		{
			if (input[*i + 2] && (input[*i + 2] == '>' || input[*i + 2] == '<'))
				return (printf("minisHell: syntax error near unexpected token `%c%c'\n", input[*i + 2], input[*i + 3]), -1);
			add_token_list(token_list, my_substr(input, *i, 2), 5);
			(*i)++;
		}
		else
			add_token_list(token_list, my_substr(input, *i, 1), 2);
		return 1;
	}
	return 0;
}

int init_op_token(char *input, int *i, t_token **token_list)
{
	if (!input || !token_list)
		return -1; // Return error if input or token_list is invalid
	// Check for specific tokens
	if (check_dollar_sign(input, i, token_list) == -1)
		return -1;
	if (check_pipe_sign(input, i, token_list) == -1)
		return -1;
	if (check_greater_than_sign(input, i, token_list) == -1)
		return -1;
	if (check_less_than_sign(input, i, token_list) == -1)
		return -1;
	return (*i);
}