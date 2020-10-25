/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/21 16:58:04 by alzaynou          #+#    #+#             */
/*   Updated: 2020/10/23 19:44:18 by alzaynou         ###   ########.fr       */
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

t_files		*cmp_mtime(t_files *f, t_files *lst)
{
	t_files		*tmp;

	tmp = lst;
	while (tmp)
	{
		if (f->l_st->st_mtime > tmp->l_st->st_mtime)
			return (tmp);
        else if (f->l_st->st_mtime == tmp->l_st->st_mtime)
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
        if (f->l_st->st_size > tmp->l_st->st_size)
            return (tmp);
        else if (f->l_st->st_size == tmp->l_st->st_size)
        {
            if (ft_strcmp(f->name, tmp->name) < 0)
                return (tmp);
        }
        tmp = tmp->next;
    }
    return (NULL);
}

t_files     *cmp_ctime(t_files *f, t_files *lst)
{
    t_files     *tmp;

    tmp = lst;
    while (tmp)
    {
        if (f->l_st->st_ctime > tmp->l_st->st_ctime)
            return (tmp);
        else if (f->l_st->st_ctime == tmp->l_st->st_ctime)
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
	if ((d->options & _T))
		return (cmp_mtime(f, lst));
	else if ((d->options & _U))
		return (cmp_ctime(f, lst));
	else if ((d->options & _S))
		return (cmp_size(f, lst));
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
    else if ((d->options & _NO_SORT))
		push_to_tail(f, l_lst);
	else
		push_sort(d, f, lst, l_lst);
}
