/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalex <lalex@students.21-school.ru>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 10:32:03 by lalex             #+#    #+#             */
/*   Updated: 2022/03/04 10:32:20 by lalex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**get_paths(char **envp)
{
	int		i;
	char	**paths;

	i = 0;
	while (envp[i] && starts_with(envp[i], "PATH=") == 0)
		i++;
	if (envp[i] == NULL)
		return (NULL);
	paths = ft_split(envp[i] + ft_strlen("PATH="), ':');
	return (paths);
}

char	**env_path(char **envp, int command)
{
	static char	**env_path;

	if (command == PATH_INIT)
		env_path = get_paths(envp);
	else if (command == PATH_READ)
		return (env_path);
	else if (command == PATH_CLEAN)
	{
		clean_strings(env_path);
		env_path = NULL;
	}
	return (env_path);
}

char	*path_join(const char *dir, const char *file)
{
	size_t	i;
	size_t	len;
	char	*path;

	len = ft_strlen(dir) + ft_strlen(file);
	path = malloc((len + 2) * sizeof(*path));
	if (!path)
		return (NULL);
	path[len + 1] = '\0';
	i = 0;
	while (dir[i])
	{
		path[i] = dir[i];
		i++;
	}
	path[i] = '/';
	len = i + 1;
	while (file[len - i - 1])
	{
		path[len] = file[len - i - 1];
		len++;
	}
	return (path);
}
