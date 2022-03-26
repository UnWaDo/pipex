/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalex <lalex@students.21-school.ru>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 10:26:41 by lalex             #+#    #+#             */
/*   Updated: 2022/03/23 23:26:53 by lalex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "pipex.h"
#include "get_next_line.h"

static void	check_args(int argc, char **argv)
{
	if (argc >= 5)
		return ;
	printf_err(BAD_USAGE_ERROR, argv[0]);
	exit(EXIT_FAILURE);
}

static int	check_file(const char *filename, int flags, int code)
{
	int	fd;

	fd = open(filename, flags, code);
	if (fd < 0
		|| ((flags & (O_RDONLY | O_RDWR)) && read(fd, NULL, 0) < 0)
		|| ((flags & (O_WRONLY | O_RDWR)) && write(fd, NULL, 0) < 0))
		printf_err(FILE_ERROR, filename, strerror(errno));
	return (fd);
}

static int	is_limiter(char *limiter, char *line)
{
	size_t	i;

	i = 0;
	while (limiter[i] && line[i] && line[i] == limiter[i])
		i++;
	return (limiter[i] == '\0' && line[i] == '\n' && line[i + 1] == '\0');
}

static int	here_doc(char *limiter)
{
	int		pipe_fds[2];
	char	*line;
	size_t	line_len;

	if (pipe(pipe_fds) == -1)
	{
		printf_err(PIPE_CREATION_ERROR, strerror(errno));
		return (-1);
	}
	line = get_next_line(0);
	line_len = ft_strlen(line);
	while (line && !is_limiter(limiter, line))
	{
		write(pipe_fds[1], line, line_len);
		free(line);
		line = get_next_line(0);
		line_len = ft_strlen(line);
	}
	if (line)
		free(line);
	close(pipe_fds[1]);
	return (pipe_fds[0]);
}

static int	check_files(int argc, char **argv, int fds[2])
{
	if (ft_strncmp(argv[1], HERE_DOC, sizeof(HERE_DOC)) == 0)
	{
		fds[0] = here_doc(argv[2]);
		fds[1] = check_file(argv[argc - 1], O_WRONLY | O_CREAT, 0644);
	}
	else
	{
		fds[0] = check_file(argv[1], O_RDONLY, 0);
		fds[1] = check_file(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	int	status;
	int	io_fds[2];

	check_args(argc, argv);
	check_files(argc, argv, io_fds);
	env_path(envp, PATH_INIT);
	if (ft_strncmp(argv[1], HERE_DOC, sizeof(HERE_DOC)) == 0)
		status = execute_commands(io_fds, argv + 3, argc - 4);
	else
		status = execute_commands(io_fds, argv + 2, argc - 3);
	env_path(envp, PATH_CLEAN);
	return (status);
}
