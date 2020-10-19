/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_intlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/15 20:30:50 by alzaynou          #+#    #+#             */
/*   Updated: 2020/10/15 20:31:22 by alzaynou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_intlen(long long int n)
{
	int					len;
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
