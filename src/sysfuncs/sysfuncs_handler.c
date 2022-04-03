/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sysfuncs_handler.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lprates <lprates@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/27 16:07:00 by lprates           #+#    #+#             */
/*   Updated: 2022/04/03 18:53:52 by lprates          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*check_sysfunction(char *func)
{
	char	*envs;
	char	**ret;
	char	*cmd;
	int		ret_access;

	envs = getenv("PATH");
	ret = ft_split(envs, ':');
	while (*ret)
	{
		cmd = ft_strjoin(*ret, ft_strjoin("/", func));
		ret_access = access(cmd, X_OK);
		if (ret_access == 0)
			return (cmd);
		ret_access = access(func, X_OK);
		if (ret_access == 0)
			return (func);
		free(cmd);
		if (*ret)
			ret++;
	}
	return (NULL);
}

/*
** funtions that handles the redirection in and append in to file
**
*/
int	send_output(int pipe, char *path, int chain)
{
	char	reading_buf[1];
	int		file;
	int		mode;

	if (chain == APPENDO)
		mode = O_RDWR | O_APPEND;
	else
		mode = O_RDWR | O_TRUNC;
	file = open(path, mode);
	if (file == -1)
		file = open(path, mode | O_CREAT, 0666);
	if (file == -1)
	{
		perror(ft_strjoin("minishell: ", path));
		return (EXIT_FAILURE);
	}
	while (read(pipe, reading_buf, 1) > 0)
	{
		write(file, reading_buf, 1);
	}
	close(file);
	return (EXIT_SUCCESS);
}

int	do_pipe(int pipe, t_command to_command)
{
	char	*cmd;

	cmd = check_sysfunction(to_command.command);
	dup2(pipe, 0);
	if (execve(cmd, to_command.args, NULL) == -1)
	{
		perror("msh");
		exit(EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	exec_sysfunction(t_command *command, char **builtin_funcs, t_environment_element *environment_linked_list)
{
	int		pid;
	int		status;
	int		my_pipe[2];

	if (pipe(my_pipe) == -1)
	{
		perror("Error creating pipe");
	}
	if (command[0].chain == 0)
		msh_execute_two(command, builtin_funcs, environment_linked_list);
	else
	{
		pid = fork();
		if (!pid)
		{
			close(my_pipe[0]);
			dup2(my_pipe[1], 1);
			msh_execute_two(command, builtin_funcs, environment_linked_list);
			close(my_pipe[1]);
			exit(0);

		}
		else if (pid < 0)
			perror("msh");
		else
		{
			waitpid(pid, &status, WUNTRACED);
			while (!WIFEXITED(status) && !WIFSIGNALED(status))
				waitpid(pid, &status, WUNTRACED);
			close(my_pipe[1]);
			if (command[0].chain == APPENDO || command[0].chain == REDIRECTO)
				send_output(my_pipe[0], command[1].args[0], command[0].chain);
			else if (command[0].chain == PIPE)
				do_pipe(my_pipe[0], command[1]);
			close(my_pipe[0]);
			ft_putstr("parent out pipe closing\n");
		}
	}
	return (0);
}
