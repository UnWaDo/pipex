/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_middle.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalex <lalex@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 21:32:18 by lalex             #+#    #+#             */
/*   Updated: 2022/03/26 21:33:57 by lalex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include "pipex.h"

static int  fork_error(int fds[2])
{
	printf_err(FORK_CREATION_ERROR, strerror(errno));
	close_pipe(fds);
	return (-1);
}

pid_t	execute_middle(int fds[2], const char *command)
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
        return (fork_error(fds));
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
