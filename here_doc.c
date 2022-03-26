/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalex <lalex@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 21:01:20 by lalex             #+#    #+#             */
/*   Updated: 2022/03/26 21:01:22 by lalex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "get_next_line.h"
#include <errno.h>
#include <string.h>

static int	is_limiter(char *limiter, char *line)
{
	size_t	i;

	i = 0;
	while (limiter[i] && line[i] && line[i] == limiter[i])
		i++;
	return (limiter[i] == '\0' && line[i] == '\n' && line[i + 1] == '\0');
}

int	here_doc(char *limiter)
{
	int		pipe_fds[2];
	char	*line;
	size_t	line_len;

	if (pipe(pipe_fds) == -1)
	{
		printf_err(PIPE_CREATION_ERROR, strerror(errno));
		return (-1);
	}
	write(1, HERE_DOC_T, sizeof(HERE_DOC_T));
	line = get_next_line(0);
	line_len = ft_strlen(line);
	while (line && !is_limiter(limiter, line))
	{
		write(pipe_fds[1], line, line_len);
		free(line);
		write(1, HERE_DOC_T, sizeof(HERE_DOC_T));
		line = get_next_line(0);
		line_len = ft_strlen(line);
	}
	if (line)
		free(line);
	close(pipe_fds[1]);
	return (pipe_fds[0]);
}
