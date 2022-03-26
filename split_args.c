/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalex <lalex@students.21-school.ru>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/09 13:41:01 by lalex             #+#    #+#             */
/*   Updated: 2022/03/04 11:03:25 by lalex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static	size_t	count_arg_length(const char *s)
{
	size_t	len;
	char	quote;

	len = 0;
	while (s[len] && ft_isspace(s[len]) == 0)
	{
		if (ft_isquote(s[len]))
		{
			quote = s[len];
			len++;
			while (s[len] && s[len] != quote)
				len++;
			if (s[len] == '\0')
				break ;
		}
		len++;
	}
	return (len);
}

static size_t	count_args(const char *s)
{
	size_t	len;
	size_t	amount;

	amount = 0;
	while (*s)
	{
		while (*s && ft_isspace(*s))
			s++;
		len = count_arg_length(s);
		if (len)
			amount++;
		s += len;
	}
	return (amount);
}

static size_t	next_arg(const char *s, char **next_arg)
{
	size_t	i;
	size_t	str_len;

	str_len = count_arg_length(s);
	*next_arg = malloc((str_len + 1) * sizeof(**next_arg));
	if (*next_arg == NULL)
		return (0);
	i = 0;
	while (i < str_len)
	{
		(*next_arg)[i] = s[i];
		i++;
	}
	(*next_arg)[i] = 0;
	return (str_len);
}

static void	remove_quotes(char *string)
{
	size_t	i;
	size_t	shift;
	char	c;

	shift = 0;
	c = 0;
	i = 0;
	while (string[i])
	{
		if ((c == string[i]) || (c == 0 && ft_isquote(string[i])))
		{
			shift++;
			if (c == 0 && ft_isquote(string[i]))
				c = string[i];
			else
				c = 0;
			i++;
			continue ;
		}
		string[i - shift] = string[i];
		i++;
	}
	string[i - shift] = 0;
}

char	**split_args(char const *s)
{
	size_t	len;
	size_t	i;
	char	**splitted;

	len = count_args(s);
	splitted = malloc((len + 1) * sizeof (*splitted));
	if (splitted == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		while (*s && ft_isspace(*s))
			s++;
		s += next_arg(s, splitted + i);
		if (splitted[i] == NULL)
		{
			clean_strings(splitted);
			return (NULL);
		}
		remove_quotes(splitted[i]);
		i++;
	}
	splitted[len] = NULL;
	return (splitted);
}
