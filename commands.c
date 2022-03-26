/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalex <lalex@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 10:32:54 by lalex             #+#    #+#             */
/*   Updated: 2022/03/26 21:25:14 by lalex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	*check_path(const char *dir, const char *file)
{
	char	*path;

	path = path_join(dir, file);
	if (path == NULL)
		return (NULL);
	if (access(path, X_OK) != -1)
		return (path);
	free(path);
	return (NULL);
}

static char	*get_full_path(char *cmd_name)
{
	char	**paths;
	char	*path;
	int		i;

	paths = env_path(NULL, PATH_READ);
	if (!paths)
		return (cmd_name);
	i = 0;
	path = NULL;
	while (paths[i])
	{
		path = check_path(paths[i], cmd_name);
		if (path)
			return (path);
		i++;
	}
	return (cmd_name);
}

char	**parse_command(const char *command)
{
	char	**args;
	char	*path;

	args = split_args(command);
	if (!args)
		return (NULL);
	if (access(args[0], X_OK) != -1)
		return (args);
	path = get_full_path(args[0]);
	if (path == NULL)
	{
		clean_strings(args);
		return (NULL);
	}
	if (path != args[0])
		free(args[0]);
	args[0] = path;
	return (args);
}

int	execute_cmd(int in_fd, int out_fd, const char *command)
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
	execve(cmd_args[0], cmd_args, env_path(NULL, ENV_READ));
	printf_err(INVALID_COMMAND_ERROR, cmd_args[0]);
	clean_strings(cmd_args);
	return (EXIT_CMD_NOT_FOUND);
}
