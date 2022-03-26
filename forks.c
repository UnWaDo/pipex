/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalex <lalex@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 10:31:51 by lalex             #+#    #+#             */
/*   Updated: 2022/03/26 21:32:10 by lalex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include "pipex.h"

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

static int	wait_all(pid_t *pids, int len)
{
	int	i;
	int	status;

	if (!pids || len == 0)
		return (EXIT_FAILURE);
	i = 0;
	while (i < len - 1)
	{
		if (pids[i] != -1)
			waitpid(pids[i], NULL, 0);
		i++;
	}
	waitpid(pids[i], &status, 0);
	free(pids);
	return (WEXITSTATUS(status));
}

static int	clean_up(pid_t *pids, int fds[2])
{
	close_pipe(fds);
	free(pids);
	return (EXIT_FAILURE);
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
		return (clean_up(pids, fds));
	pids[i] = execute_last(fds, commands[i], &status);
	close_pipe(fds);
	if (pids[i] <= 0)
	{
		free(pids);
		return (status);
	}
	return (wait_all(pids, cmdc));
}
