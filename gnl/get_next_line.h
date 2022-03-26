/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalex <lalex@students.21-school.ru>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 21:58:46 by lalex             #+#    #+#             */
/*   Updated: 2022/03/10 10:30:37 by lalex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stddef.h>
# include <limits.h>
# include <unistd.h>
# include <stdlib.h>

char	*get_next_line(int fd);

typedef struct s_fdlst {
	int				fd;
	char			*buffer;
	size_t			buffer_shift;
	size_t			buffer_size;
	size_t			line_length;
	struct s_fdlst	*next;
}	t_fdlst;

// Searches for t_fdlst with file descriptor `fd`
// If not found, creates new t_fdlst element and
// adds it to the beginning of `lst`
// Finally, allocates the buffer for t_fdlst
// of size BUFFER_SIZE + 1
// If allocation fails on any stage, the memory
// is freed and the NULL returns
t_fdlst	*prepare_cfd(int fd, t_fdlst **lst);

// Removes t_fdlst with file descriptor `fd` from list
// and frees `buffer` and itself
// If `*lst` is NULL, does nothing
// Always returns NULL
void	*pop_fd(t_fdlst **lst, int fd);

// Allocates line from `lst->buffer` including `lst->line_length` symbols
// from the beginning. After this modifies `lst->buffer_shift`, 
// `lst->buffer_size` and `lst->line_length` to satisfy new reality
// If allocation fails or if either `lst` or `lst->buffer` is NULL
// does nothing and returns NULL
char	*pop_line(t_fdlst *cfd);

// Expand buffer located in cfd to hold `BUFFER_SIZE` more characters
// Valuable characters inside `cfd->buffer` are saved
// If allocation fails frees `cfd->buffer` and sets it to NULL
// If `cfd` is NULL does nothing
void	expand_buffer(t_fdlst *cfd);

// Goes through buffer starting with
// index `cfd->line_length` looking for newline character
// Sets `cfd->line_length` value to the length of line
// to be obtained with `pop_line` function
void	scan_buffer(t_fdlst *cfd);

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1000
# endif

#endif