/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalex <lalex@students.21-school.ru>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 14:02:42 by lalex             #+#    #+#             */
/*   Updated: 2022/03/10 10:19:41 by lalex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LST_H
# define FT_LST_H

# include <stdlib.h>

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

// Creates new element with content `content`
// Returns pointer to allocated element or NULL if allocation fails
t_list	*ft_lstnew(void *content);

// Adds element `new` to the beginning of the list
// Does nothing if `new` is NULL
void	ft_lstadd_front(t_list **lst, t_list *new);

// Frees the memory of element's content with function `del`
// and frees the element's memory
// `lst->next` is not freed
// If `lst` is NULL, does nothing
void	ft_lstdelone(t_list *lst, void (*del)(void *));

// Frees the memory of element and all its successors
// using functions `del` and `free`
// The pointer to list is set to NULL afterwards
void	ft_lstclear(t_list **lst, void (*del)(void *));

#endif