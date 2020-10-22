/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/21 16:58:04 by alzaynou          #+#    #+#             */
/*   Updated: 2020/10/22 19:34:18 by alzaynou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		sort_ctime(t_files *f, t_files **lst, t_files **l_lst)
{
	t_files		*tmp;
	if (!*lst)
	{
		*lst = f;
		*l_lst = f;
	}
	else
	{
		tmp = *lst;
//		if (
	}
}

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
		*lst = f;
	}
}

void		push_to_tail(t_files *f, t_files **l_lst)
{
	*l_lst->next = f;
	*l_lst = f;
}

void		sort_ascii(t_flies *f, t_files **lst, t_files **l_lst)
{
	t_files		*tmp;

	if (!*lst)
		push_to_head(f, lst, l_lst);
	else
	{
		if (ft_strcmp(f->name, *lst->name) < 0)
			push_to_head(f, lst, l_lst);
		else
		{
			if (!*lst->next)
				push_to_tail(f, l_lst);
			else
			{
				tmp = *lst;
				while (tmp->next)
				{
					if (ft_strcmp(f->name, tmp->next->name) < 0)
					{
						f->next = tmp->next;
						tmp->next = f;
						break ;
					}
					tmp = tmp->next;
				}
				if (!tmp->next)
					push_to_tail(f, l_lst);
			}
		}
	}
}

void		push_sort(t_all *d, t_files *f, t_files **lst, t_files **l_lst)
{
	if ((d->options & _U))
		sort_ctime(f, lst, l_lst);
//	else if ((d->options & _S))
//		sort_size(f, lst, l_lst);
//	else if ((d->options & _T))
//		sort_mtime(f, lst, l_lst);
	else
		sort_ascii(f, lst, l_lst);
}

void		push_files(t_all *d, t_files *f, t_files **lst, t_files **l_lst)
{
	if ((d->options & _NO_SORT))
	{
		if (!*lst)
			*lst = f;
		*l_st = f;
	}
	else
		push_sort(d, f, lst, l_lst);
}
