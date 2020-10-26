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

t_waiting		*init_waiting(t_all *d, t_files *f)
{
	t_waiting		*new;

	if (!(new = (t_waiting *)ft_memalloc(sizeof(t_waiting))))
		error_ls(d, strerror(errno));
    new->name = f->name;
    new->st = f->st;
    new->l_st = f->l_st;
    f->name = NULL;
    f->st = NULL;
    f->l_st = NULL;
    ft_printf("push %s\n", new->name);
    return (new);
}

void		push_waiting(t_all *d, t_files *f)
{
	t_waiting		*new;

	new = init_waiting(d, f);
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
