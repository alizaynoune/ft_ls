/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_dir.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/25 19:26:54 by alzaynou          #+#    #+#             */
/*   Updated: 2020/10/27 01:59:41 by alzaynou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

t_waiting		*init_waiting(t_all *d, t_files *f, char **name)
{
	t_waiting		*new;

	if (!(new = (t_waiting *)ft_memalloc(sizeof(t_waiting))))
		error_ls(d, strerror(errno));
    new->name = *name;
    new->st = f->st;
    *name = NULL;
    f->st = NULL;
    return (new);
}

void		push_waiting(t_all *d, t_files *f)
{
	t_waiting		*new;

	new = init_waiting(d, f, &f->name);
    if (!d->head_waiting)
    {
        d->head_waiting = new;
        d->lst_waiting = new;
    }
    else
    {
        d->lst_waiting->next = new;
        d->lst_waiting = new;
    }
}
