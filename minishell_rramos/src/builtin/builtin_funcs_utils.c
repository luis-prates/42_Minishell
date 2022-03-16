/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_funcs_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lprates <lprates@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/27 16:25:31 by lprates           #+#    #+#             */
/*   Updated: 2022/03/13 20:46:50 by lprates          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_builtin_funcs(char **builtin_funcs)
{
	builtin_funcs[0] = "cd";
	builtin_funcs[1] = "pwd";
	builtin_funcs[2] = "echo";
	builtin_funcs[3] = "export";
	builtin_funcs[4] = "unset";
	builtin_funcs[5] = "env";
	builtin_funcs[6] = "exit";
}

/* hub for executing builtin functions
** returns 1 on success
** returns 0 on failure
** needs error handling
** export and unset missing
*/
int	execute_builtins(char *cmd, char **args, char **environ)
{
	if (!ft_strcmp(cmd, "cd"))
		do_cd(args[1]);
	if (!ft_strcmp(cmd, "pwd"))
		printf("current path is: %s\n", getcwd(NULL, 0));
	if (!ft_strcmp(cmd, "echo"))
		do_echo(args);
	if (!ft_strcmp(cmd, "env"))
		while (*environ)
			printf("%s\n", *(environ++));
	if (!ft_strcmp(cmd, "exit"))
		do_exit(args);
	return (1);
}

int	builtin(t_command *cmd, char **builtin_funcs, char **environ)
{
	int	idx;

	idx = -1;
	while (++idx < BUILTIN_FUNCS_NB)
	{
		if (!ft_strcmp(cmd->command, builtin_funcs[idx]))
		{
			execute_builtins(cmd->command, cmd->args, environ);
			return (1);
		}
	}
	return (0);
}