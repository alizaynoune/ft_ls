/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/08 11:39:00 by alzaynou          #+#    #+#             */
/*   Updated: 2020/11/08 11:56:46 by alzaynou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		*ft_memdup(const void *ptr, size_t size)
{
	void		*new;

	if (!(new = malloc(sizeof(void *) * size)))
		return (NULL);
	new = ft_memcpy(new, ptr, size);
	return (new);
}
