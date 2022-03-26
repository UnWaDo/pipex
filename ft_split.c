/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalex <lalex@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/09 13:41:01 by lalex             #+#    #+#             */
/*   Updated: 2022/03/26 21:08:30 by lalex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static size_t	count_strings(const char *s, char c)
{
	size_t	i;
	size_t	len;
	size_t	amount;

	i = 0;
	amount = 0;
	while (s[i])
	{
		while (s[i] && (s[i] == c))
			i++;
		len = 0;
		while (s[i + len] && s[i + len] != c)
			len++;
		if (len)
			amount++;
		i += len;
	}
	return (amount);
}

static char	*next_string(const char *s, size_t *shift, char c)
{
	size_t	i;
	size_t	str_len;
	char	*new_str;

	while (s[*shift] == c)
		(*shift)++;
	str_len = 0;
	while (s[*shift + str_len] && s[*shift + str_len] != c)
		str_len++;
	new_str = malloc((str_len + 1) * sizeof(*new_str));
	i = 0;
	while (i < str_len)
	{
		new_str[i] = s[*shift];
		(*shift)++;
		i++;
	}
	new_str[str_len] = 0;
	return (new_str);
}

char	**ft_split(char const *s, char c)
{
	size_t	len;
	size_t	i;
	char	**splitted;
	size_t	shift;

	if (!s)
		return (NULL);
	len = count_strings(s, c);
	splitted = malloc((len + 1) * sizeof (*splitted));
	if (splitted == NULL)
		return (NULL);
	i = 0;
	shift = 0;
	while (i < len)
	{
		splitted[i] = next_string(s, &shift, c);
		if (splitted[i] == NULL)
		{
			clean_strings(splitted);
			return (NULL);
		}
		i++;
	}
	splitted[len] = NULL;
	return (splitted);
}
