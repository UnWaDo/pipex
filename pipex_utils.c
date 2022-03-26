/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalex <lalex@students.21-school.ru>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 10:32:29 by lalex             #+#    #+#             */
/*   Updated: 2022/03/04 10:32:30 by lalex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	clean_strings(char **splitted)
{
	size_t	i;

	i = 0;
	while (splitted[i] != NULL)
		free(splitted[i++]);
	free(splitted);
}

int	ft_isspace(int c)
{
	if (c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r' || c == ' ')
		return (1);
	return (0);
}

int	ft_isquote(int c)
{
	if (c == '\'' || c == '"')
		return (1);
	return (0);
}

void	close_pipe(int fds[2])
{
	if (fds[0] != -1)
		close(fds[0]);
	if (fds[1] != -1)
		close(fds[1]);
}