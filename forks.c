/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalex <lalex@students.21-school.ru>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 10:31:51 by lalex             #+#    #+#             */
/*   Updated: 2022/03/23 23:07:39 by lalex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include "pipex.h"
#include <sys/wait.h>
#include "ft_lst.h"

extern char	**environ;

static int	execute_cmd(int in_fd, int out_fd, const char *command)
{
	char	**cmd_args;
	int		fds[2];

	fds[0] = in_fd;
	fds[1] = out_fd;
	cmd_args = parse_command(command);
	if (!cmd_args)
		return (EXIT_FAILURE);
	if (fds[0] == -1 || fds[1] == -1)
	{
		close_pipe(fds);
		clean_strings(cmd_args);
		return (EXIT_FAILURE);
	}
	if (dup2(fds[0], 0) == -1 || dup2(fds[1], 1) == -1)
	{
		close_pipe(fds);
		return (EXIT_FAILURE);
	}
	close_pipe(fds);
	execve(cmd_args[0], cmd_args, environ);
	printf_err(INVALID_COMMAND_ERROR, cmd_args[0]);
	clean_strings(cmd_args);
	return (EXIT_CMD_NOT_FOUND);
}

static pid_t	execute_middle(int fds[2], const char *command)
{
	int		pipe_fds[2];
	pid_t	child;

	if (pipe(pipe_fds) == -1)
	{
		printf_err(PIPE_CREATION_ERROR, strerror(errno));
		return (-1);
	}
	child = fork();
	if (child == -1)
	{
		printf_err(FORK_CREATION_ERROR, strerror(errno));
		return (-1);
	}
	else if (child == 0)
	{
		close(pipe_fds[0]);
		execute_cmd(fds[0], pipe_fds[1], command);
		close(pipe_fds[1]);
		return (-1);
	}
	close(pipe_fds[1]);
	if (fds[0] != -1)
		close(fds[0]);
	fds[0] = pipe_fds[0];
	return (child);
}

static pid_t	execute_last(int fds[2], const char *command, int *status)
{
	pid_t	child;

	*status = 0;
	child = fork();
	if (child == -1)
	{
		printf_err(FORK_CREATION_ERROR, strerror(errno));
		*status = EXIT_FAILURE;
		return (-1);
	}
	else if (child == 0)
		*status = execute_cmd(fds[0], fds[1], command);
	return (child);
}

int	execute_commands(int fds[2], char **commands, int cmdc)
{
	pid_t	*pids;
	int		i;
	int		status;

	pids = malloc(sizeof(*pids) * cmdc);
	if (!pids)
		return (0);
	i = 0;
	while (i < cmdc - 1)
	{
		pids[i] = execute_middle(fds, commands[i]);
		if (pids[i] < 0)
			break ;
		i++;
	}
	if (i < cmdc - 1)
	{
		close_pipe(fds);
		free(pids);
		return (EXIT_FAILURE);
	}
	pids[i] = execute_last(fds, commands[i], &status);
	close_pipe(fds);
	if (pids[i] == 0)
	{
		free(pids);
		return (status);
	}
	i = 0;
	while (i < cmdc)
	{
		if (pids[i] != -1)
			waitpid(pids[i++], &status, 0);
	}
	free(pids);
	if (status)
		return (WEXITSTATUS(status));
	return (status);
}
