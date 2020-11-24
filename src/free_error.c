/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/24 19:45:15 by alzaynou          #+#    #+#             */
/*   Updated: 2020/11/24 20:51:36 by alzaynou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		free_pwd(struct passwd **pwd)
{
	((*pwd)->pw_name) ? ft_strdel(&(*pwd)->pw_name) : 0;
	(*pwd) ? ft_memdel((void *)&(*pwd)) : 0;
}

void		free_grp(struct group **grp)
{
	((*grp)->gr_name) ? ft_strdel(&(*grp)->gr_name) : 0;
	(*grp) ? ft_memdel((void *)&(*grp)) : 0;
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
		(tmp->st) ? ft_memdel((void *)&tmp->st) : 0;
		(tmp->pwd) ? free_pwd(&tmp->pwd) : 0;
		(tmp->grp) ? free_grp(&tmp->grp) : 0;
		(tmp->link) ? ft_strdel(&tmp->link) : 0;
		ft_memdel((void *)&tmp);
	}
	(*lst) = NULL;
	errno = 0;
}

void		free_dir(t_dir **dir)
{
	if (!*dir)
		return ;
	((*dir)->h_files) ? free_files(&(*dir)->h_files) : 0;
	((*dir)->path) ? ft_memdel((void *)&(*dir)->path) : 0;
	(*dir) ? ft_memdel((void *)&(*dir)) : 0;
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
		(tmp->st) ? ft_memdel((void *)&tmp->st) : 0;
		ft_memdel((void *)&tmp);
	}
	(*lst) = NULL;
}

void		free_all(t_all *d)
{
	(d->arg_file) ? free_files(&d->arg_file) : 0;
	(d->files) ? free_files(&d->files) : 0;
	(d->head_waiting) ? free_waiting(&d->head_waiting) : 0;
	(d->dir) ? free_dir(&d->dir) : 0;
	(d) ? ft_memdel((void *)&d) : 0;
}

