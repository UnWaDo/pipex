/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalex <lalex@students.21-school.ru>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 10:32:54 by lalex             #+#    #+#             */
/*   Updated: 2022/03/04 11:55:35 by lalex            ###   ########.fr       */
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
		return (NULL);
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
