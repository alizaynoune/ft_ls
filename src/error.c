/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 11:53:05 by alzaynou          #+#    #+#             */
/*   Updated: 2020/10/22 16:50:45 by alzaynou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		free_files(t_files *lst)
{
	t_files		*tmp;

	while (lst)
	{
		tmp = lst;
		lst = lst->next;
		(tmp->name) ? free(tmp->name) : 0;
		(tmp->dirent) ? free(tmp->dirent) : 0;
		(tmp->st) ? free(tmp->st) : 0;
		(tmp->l_st) ? free(tmp->l_st) : 0;
		free(tmp);
	}
}

void		free_waiting(t_waiting *lst)
{
	t_waiting		*tmp;

	while (lst)
	{
		tmp = lst;
		lst = lst->next;
		(tmp->name) ? free(tmp->name) : 0;
		(tmp->path) ? free(tmp->path) : 0;
		(tmp->st) ? free(tmp->st) : 0;
		(tmp->l_st) ? free(tmp->l_st) : 0;
		free(tmp);
	}
}

void		free_all(t_all *d)
{
	(d->files) ? free_files(d->files) : 0;
	(d->head_waiting) ? free_waiting(d->head_waiting) : 0;
	free(d);
}

int			error_ls(t_all *d,  char *err)
{
	ft_dprintf(0, "%s\n", err);
	(d->files) ? free_files(d->files) : 0;
	(d->head_waiting) ? free_waiting(d->head_waiting) : 0;
	free(d);
	exit(_TROUBLE);
}


