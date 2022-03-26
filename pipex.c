/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalex <lalex@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 21:20:03 by lalex             #+#    #+#             */
/*   Updated: 2022/03/26 21:20:04 by lalex            ###   ########.fr       */
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
	if (argc >= 6)
		return ;
	if (argc >= 5 && ft_strncmp(argv[1], HERE_DOC, sizeof(HERE_DOC)))
		return ;
	printf_err(BAD_USAGE_ERROR, argv[0]);
	exit(EXIT_FAILURE);
}

static int	check_file(const char *filename, int flags, int code)
{
	int	fd;

	fd = open(filename, flags, code);
	if (fd < 0)
		printf_err(FILE_ERROR, strerror(errno), filename);
	return (fd);
}

static int	input_files(int argc, char **argv, int fds[2])
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
	input_files(argc, argv, io_fds);
	env_path(envp, PATH_INIT);
	if (ft_strncmp(argv[1], HERE_DOC, sizeof(HERE_DOC)) == 0)
		status = execute_commands(io_fds, argv + 3, argc - 4);
	else
		status = execute_commands(io_fds, argv + 2, argc - 3);
	env_path(envp, PATH_CLEAN);
	return (status);
}
