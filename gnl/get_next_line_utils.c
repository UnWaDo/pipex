/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalex <lalex@students.21-school.ru>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 11:06:44 by lalex             #+#    #+#             */
/*   Updated: 2021/10/21 13:23:38 by lalex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

t_fdlst	*prepare_cfd(int fd, t_fdlst **lst)
{
	t_fdlst			*cfd;

	cfd = *lst;
	while (cfd && cfd->fd != fd)
		cfd = cfd->next;
	if (cfd)
		return (cfd);
	cfd = malloc(sizeof (*cfd));
	if (!cfd)
		return (NULL);
	cfd->fd = fd;
	cfd->next = *lst;
	*lst = cfd;
	cfd->line_length = 0;
	cfd->buffer = malloc(((size_t)(BUFFER_SIZE)) + 1);
	cfd->buffer_shift = 0;
	cfd->buffer_size = 0;
	if (cfd->buffer == NULL)
		return (pop_fd(lst, fd));
	cfd->buffer[cfd->buffer_size] = 0;
	return (cfd);
}

void	*pop_fd(t_fdlst **lst, int fd)
{
	t_fdlst	*previous;
	t_fdlst	*current;

	current = *lst;
	while (current && current->fd != fd)
	{
		previous = current;
		current = current->next;
	}
	if (!current)
		return (NULL);
	if (current == *lst)
		*lst = current->next;
	else
		previous->next = current->next;
	if (current->buffer)
		free(current->buffer);
	free(current);
	return (NULL);
}

char	*pop_line(t_fdlst *cfd)
{
	char	*str;
	size_t	pos;

	if (cfd == NULL || cfd->buffer == NULL)
		return (NULL);
	str = malloc(cfd->line_length + 1);
	if (!str)
		return (NULL);
	str[cfd->line_length] = 0;
	pos = 0;
	while (pos < cfd->line_length)
	{
		str[pos] = cfd->buffer[cfd->buffer_shift + pos];
		pos++;
	}
	cfd->buffer_shift += cfd->line_length;
	cfd->buffer_size -= cfd->line_length;
	cfd->line_length = 0;
	return (str);
}

void	expand_buffer(t_fdlst *cfd)
{
	char	*new;
	size_t	pos;

	if (cfd == NULL || cfd->buffer == NULL)
		return ;
	new = malloc(cfd->buffer_size + ((size_t)(BUFFER_SIZE)) + 1);
	if (new == NULL)
	{
		free(cfd->buffer);
		cfd->buffer = NULL;
		cfd->buffer_size = 0;
		return ;
	}
	pos = 0;
	while (pos < cfd->buffer_size)
	{
		new[pos] = cfd->buffer[cfd->buffer_shift + pos];
		pos++;
	}
	free(cfd->buffer);
	cfd->buffer = new;
	cfd->buffer_shift = 0;
}

void	scan_buffer(t_fdlst *cfd)
{
	size_t	pos;

	pos = cfd->line_length;
	while (pos < cfd->buffer_size)
	{
		pos++;
		if (cfd->buffer[cfd->buffer_shift + pos - 1] == '\n')
			break ;
	}
	cfd->line_length = pos;
}
