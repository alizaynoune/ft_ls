/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 11:53:05 by alzaynou          #+#    #+#             */
/*   Updated: 2020/10/31 00:58:05 by alzaynou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		free_pwd(struct passwd **pwd)
{
	((*pwd)->pw_name) ? ft_strdel(&(*pwd)->pw_name) : 0;
	ft_voidfree((void *)&(*pwd));
}

void		free_grp(struct group **grp)
{
	((*grp)->gr_name) ? ft_strdel(&(*grp)->gr_name) : 0;
	ft_voidfree((void *)&(*grp));
}

void		free_files(t_files **lst)
{
	t_files		*tmp;

	while (*lst)
	{
		tmp = *lst;
		*lst = (*lst)->next;
		(tmp->name) ? ft_strdel(&tmp->name) : 0;
		(tmp->path) ? ft_strdel(&tmp->path) : 0;
		(tmp->st) ? ft_voidfree((void *)&tmp->st) : 0;
		(tmp->pwd) ? free_pwd(&tmp->pwd) : 0;
		(tmp->grp) ? free_grp(&tmp->grp) : 0;
		(tmp->link) ? ft_strdel(&tmp->link) : 0;
        ft_voidfree((void *)&tmp);
	}
	(*lst) = NULL;
	errno = 0;
}

void		free_dir(t_dir **dir)
{
	if (!*dir)
		return ;
	((*dir)->h_files) ? free_files(&(*dir)->h_files) : 0;
	((*dir)->path) ? ft_voidfree((void *)&(*dir)->path) : 0;
	ft_voidfree((void *)&(*dir));
}

void		free_waiting(t_waiting **lst)
{
	t_waiting		*tmp;

	while (*lst)
	{
		tmp = *lst;
		*lst = (*lst)->next;
		(tmp->name) ? ft_strdel(&tmp->name) : 0;
		(tmp->full_name) ? ft_strdel(&tmp->full_name) : 0;
		(tmp->st) ? ft_voidfree((void *)&tmp->st) : 0;
		ft_voidfree((void *)&tmp);
	}
	(*lst) = NULL;
}

void		free_all(t_all *d)
{
	(d->arg_file) ? free_files(&d->arg_file) : 0;
	(d->files) ? free_files(&d->files) : 0;
	(d->head_waiting) ? free_waiting(&d->head_waiting) : 0;
	(d->dir) ? free_dir(&d->dir) : 0;
	ft_voidfree((void *)&d);
}

int			error_ls(t_all *d,  char *err)
{
    errno = 0;
	ft_dprintf(2, "%s\n", err);
	if (d->fd_dir)
		((closedir(d->fd_dir) == -1)) ? ft_dprintf(2, "ls: connot close dir %s",
		   	strerror(errno)) : 0;
	free_all(d);
	exit(_TROUBLE);
}

