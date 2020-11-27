/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 10:41:47 by alzaynou          #+#    #+#             */
/*   Updated: 2020/11/27 10:46:18 by alzaynou         ###   ########.fr       */
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
