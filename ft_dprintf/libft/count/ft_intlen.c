/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_intlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/15 20:30:50 by alzaynou          #+#    #+#             */
/*   Updated: 2020/11/27 11:55:18 by alzaynou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t			ft_intlen(long long int n)
{
	size_t				len;
	unsigned long long	nb;

	len = n < 0 ? 1 : 0;
	nb = n < 0 ? n * -1 : n;
	while (nb > 0)
	{
		nb /= 10;
		len++;
	}
	return (len);
}

size_t			ft_uintlen(unsigned long long int n)
{
	size_t		len;

	len = 0;
	while (n > 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}
