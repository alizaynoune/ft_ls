/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/25 19:08:22 by alzaynou          #+#    #+#             */
/*   Updated: 2020/10/27 02:42:29 by alzaynou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		print_files(t_all *d, t_files *f)
{
	if ((d->options & _L))
	{
		//print mode
		// print nlink
		// print owner
		// print group
		// print size
		// print time
		ft_printf("%s  lllll\n", f->name);
        // if is link print name of stat file
	}
	else
		ft_printf("%s\n", f->name);
}

t_waiting		*new_waiting(t_all *d, t_files *f, t_waiting *curr)
{
	t_waiting		*new;
	char			*n_name;

	if (!ft_strcmp(f->name, ".") || !ft_strcmp(f->name, ".."))
		return (NULL);
//	n_name = ft_nstrjoin(3, d->dir->path, "/", f->name);//erorr
	new = init_waiting(d, f, &f->path);
	return (new);
}

void		loop_print_files(t_all *d, t_files *lst, t_waiting *curr)
{
	t_files		*tmp;
	t_waiting	*h_w;
	t_waiting	*l_w;

	h_w = NULL;
	l_w = NULL;
	tmp = lst;
	while (tmp)
	{
		print_files(d, tmp);
		if ((d->options & _R_) && ((tmp->st->st_mode & S_IFMT) == S_IFDIR))
		{
			if (h_w)
				((l_w->next = new_waiting(d, tmp, curr))) ? l_w = l_w->next : 0;
			else
				((h_w = new_waiting(d, tmp, curr))) ? l_w = h_w : 0;
		}
		tmp = tmp->next;
	}
	if (h_w)
	{
		l_w->next = curr->next;
		curr->next = h_w;
	}
}
