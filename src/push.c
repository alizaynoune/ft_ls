/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/21 16:58:04 by alzaynou          #+#    #+#             */
/*   Updated: 2020/11/24 20:44:21 by alzaynou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		push_to_tail(t_files *f, t_files **l_lst)
{
	(*l_lst)->next = f;
	f->prev = *l_lst;
	*l_lst = f;
}


void		push_sort(t_all *d, t_files *f, t_files **lst, t_files **l_lst)
{
	t_files		*tmp;
	t_files		*prev;


	if ((tmp = get_position(d, f, *lst)))
	{
		prev = tmp->prev;
		(prev) ? prev->next = f : 0;
		(!prev) ? *lst = f : 0;
		f->prev = prev;
		f->next = tmp;
		tmp->prev = f;
	}
	else
		push_to_tail(f, l_lst);
}

void		push_files(t_all *d, t_files *f, t_files **lst, t_files **l_lst)
{
	if (!*lst)
	{
		*lst = f;
		*l_lst = f;
	}
	else if ((d->options & _F))
		push_to_tail(f, l_lst);
	else
		push_sort(d, f, lst, l_lst);
}

void        push_waiting(t_all *d, t_files *f)
{
	t_waiting       *new;

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

t_waiting       *init_waiting(t_all *d, t_files *f)
{
	t_waiting       *new;

	if (!(new = (t_waiting *)ft_memalloc(sizeof(t_waiting))))
		error_ls(d, strerror(errno));
	new->name = f->name;
	new->full_name = f->path;
	new->st = f->st;
	f->name = NULL;
	f->path = NULL;
	f->st = NULL;
	return (new);
}
