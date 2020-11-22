/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/21 16:58:04 by alzaynou          #+#    #+#             */
/*   Updated: 2020/11/08 13:44:20 by alzaynou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		push_to_tail(t_files *f, t_files **l_lst)
{
	(*l_lst)->next = f;
	f->prev = *l_lst;
	*l_lst = f;
}

t_files		*cmp_ascii(t_files *f, t_files *lst)
{
	t_files		*tmp;

	tmp = lst;
	while (tmp)
	{
		if (ft_strcmp(f->name, tmp->name) < 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

t_files		*cmp_time(t_files *f, t_files *lst, int options)
{
	t_files		*tmp;
    time_t      f_time;
    time_t      t_time;

	tmp = lst;
    f_time = (options & _U) ? f->st->st_atime : f->st->st_mtime;
	while (tmp)
	{
        t_time = (options & _U) ? tmp->st->st_atime : tmp->st->st_mtime;
		if (f_time > t_time)
			return (tmp);
        else if (f_time == t_time)
        {
            if (ft_strcmp(f->name, tmp->name) < 0)
                return (tmp);
        }
		tmp = tmp->next;
	}
	return (NULL);
}

t_files     *cmp_size(t_files *f, t_files *lst)
{
    t_files     *tmp;

    tmp = lst;
    while (tmp)
    {
        if (f->st->st_size > tmp->st->st_size)
            return (tmp);
        else if (f->st->st_size == tmp->st->st_size)
        {
            if (ft_strcmp(f->name, tmp->name) < 0)
                return (tmp);
        }
        tmp = tmp->next;
    }
    return (NULL);
}

t_files		*get_position(t_all *d, t_files *f, t_files *lst)
{
	if ((d->options & _S_))
		return (cmp_size(f, lst));
	else if ((d->options & _T))
		return (cmp_time(f, lst, d->options));
    else
        return (cmp_ascii(f, lst));
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
