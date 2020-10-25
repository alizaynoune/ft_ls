/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_dir.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/25 19:26:54 by alzaynou          #+#    #+#             */
/*   Updated: 2020/10/25 20:16:19 by alzaynou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	push_dir(t_all *d, t_files *f)
{
	t_waiting		*dir;
//	dir = init_dir(d);
}

t_waiting		*init_waiting(t_all *d)
{
	t_waiting		*new;

	if (!(new = (t_waiting *)ft_memalloc(sizeof(t_waiting))))
		error_ls(d, strerror(errno));
	if (!(
}

void		push_waiting(t_all *d, t_files **f)
{
	t_waiting		*new;

	new = init_waiting(d, f);
}
