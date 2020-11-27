/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_extended.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 10:41:56 by alzaynou          #+#    #+#             */
/*   Updated: 2020/11/27 10:48:22 by alzaynou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void			free_waiting(t_waiting **lst)
{
	t_waiting		*tmp;

	while (*lst)
	{
		tmp = *lst;
		*lst = (*lst)->next;
		(tmp->name) ? ft_strdel(&tmp->name) : 0;
		(tmp->full_name) ? ft_strdel(&tmp->full_name) : 0;
		(tmp->st) ? ft_memdel((void *)&tmp->st) : 0;
		ft_memdel((void *)&tmp);
	}
	(*lst) = NULL;
}

void			free_all(t_all *d)
{
	(d->arg_file) ? free_files(&d->arg_file) : 0;
	(d->files) ? free_files(&d->files) : 0;
	(d->head_waiting) ? free_waiting(&d->head_waiting) : 0;
	(d->dir) ? free_dir(&d->dir) : 0;
	(d) ? ft_memdel((void *)&d) : 0;
}

int				error_ls(t_all *d, char *err)
{
	errno = 0;
	ft_dprintf(_ERR, "%s\n", err);
	if (d->fd_dir)
		((closedir(d->fd_dir) == -1)) ?
			ft_dprintf(_ERR, "ls: connot close dir %s", strerror(errno)) : 0;
	free_all(d);
	exit(_TROUBLE);
}

void			error_read_link(t_all *d, char *name)
{
	ft_dprintf(_ERR, "readlink: '%s'  %s\n", strerror(errno), name);
	d->ret = _FAILURE;
	errno = 0;
}

void			free_printed(t_all *d)
{
	t_waiting		*tmp;

	tmp = d->head_waiting;
	d->head_waiting = d->head_waiting->next;
	if (!d->head_waiting)
		d->lst_waiting = NULL;
	tmp->next = NULL;
	free_waiting(&tmp);
}
