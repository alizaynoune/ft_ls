/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 10:43:19 by alzaynou          #+#    #+#             */
/*   Updated: 2020/11/27 11:45:40 by alzaynou         ###   ########.fr       */
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

void		push_recursuvely(t_waiting *last, t_waiting *head, t_waiting *curr)
{
	last->next = curr->next;
	curr->next = head;
}
