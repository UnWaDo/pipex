/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_min.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalex <lalex@students.21-school.ru>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 10:32:39 by lalex             #+#    #+#             */
/*   Updated: 2022/03/04 11:49:08 by lalex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include "pipex.h"

static int	print_s(char *str)
{
	return (write(2, str, ft_strlen(str)));
}

#define MAX_INT_LEN	10

static int	print_d(int d)
{
	char	num[MAX_INT_LEN];
	size_t	len;

	if (d >= 0)
		d = -d;
	else
		write(2, "-", 1);
	len = 1;
	while (d / 10)
	{
		num[MAX_INT_LEN - len] = -(d % 10) + '0';
		d /= 10;
		len++;
	}
	num[MAX_INT_LEN - len] = -(d % 10) + '0';
	return (write(2, num + MAX_INT_LEN - len, len));
}

int	printf_err(const char *s, ...)
{
	va_list	args;
	size_t	len;
	size_t	last_out;
	int		out;

	va_start(args, s);
	len = 0;
	last_out = 0;
	out = 0;
	while (s[len])
	{
		if (s[len] == '%')
		{
			out += write(2, s + last_out, len - last_out);
			if (s[len + 1] == 's')
				out += print_s(va_arg(args, char *));
			if (s[len + 1] == 'd')
				out += print_d(va_arg(args, int));
			len += 1;
			last_out = len + 1;
		}
		len++;
	}
	out += write(2, s + last_out, len - last_out);
	return (out);
}
