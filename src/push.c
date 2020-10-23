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

void		push_to_head(t_files *f, t_files **lst, t_files **l_lst)
{
	if (!*lst)
	{
		*lst = f;
		*l_lst = f;
	}
	else
	{
		f->next = *lst;
		(*lst)->prev = f;
		*lst = f;
	}
}

void		push_to_tail(t_files *f, t_files **l_lst)
{
	(*l_lst)->next = f;
	f->prev = *l_lst;
	*l_lst = f;
}

int			cmp_head(t_all *d, t_files *f, t_files *head)
{
	int		ret;

	ret = 0;
	if (d->options & _T)
		ret = (f->l_st->st_mtime > head->l_st->st_mtime) ? -1 : 0;
	else if (d->options & _U)
	   ret = (f->l_st->st_ctime > head->l_st->st_ctime) ? -1 : 0;
	else if (d->options & _S)
		ret = (f->l_st->st_size > head->l_st->st_size) ? -1 : 0;
	else
		ret = (ft_strcmp(f->name, head->name));
	return (ret);
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
		tmp = tmp->next;
	}
	return (NULL);
}

t_files		*get_position(t_all *d, t_files *f, t_files *lst)
{
	if ((d->options & _T))
		return (cmp_mtime(f, lst));
/*	else if ((d->options & _U))
		return (cmp_ctime(f, lst));
	else if ((d->options & _S))
		return (cmp_size(f, lst));
	 else
*/		 return (cmp_ascii(f, lst));
}

void		push_sort(t_all *d, t_files *f, t_files **lst, t_files **l_lst)
{
	t_files		*tmp;
	t_files		*prev;
	
	if (!*lst)
		push_to_head(f, lst, l_lst);
	else if (cmp_head(d, f, *lst) < 0)
		push_to_head(f, lst, l_lst);
	else if ((tmp = get_position(d, f, *lst)))
	{
		prev = tmp->prev;
		prev->next = f;
		f->prev = prev;
		f->next = tmp;
		tmp->prev = f;
	}
	else
		push_to_tail(f, l_lst);
}

void		push_files(t_all *d, t_files *f, t_files **lst, t_files **l_lst)
{
	if ((d->options & _NO_SORT))
		push_to_tail(f, l_lst);
	else
		push_sort(d, f, lst, l_lst);
}
