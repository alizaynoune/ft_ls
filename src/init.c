/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/24 19:33:33 by alzaynou          #+#    #+#             */
/*   Updated: 2020/11/24 20:43:07 by alzaynou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int     init_pwd(t_all *d, t_files *new)
{
	struct passwd       *pwd;

	errno = 0;
	pwd = getpwuid(new->st->st_uid);
	if (errno)
	{
		ft_dprintf(_ERR, "getpwuid: %s", strerror(errno));
		errno = 0;
		d->ret = _FAILURE;
		return (_FAILURE);
	}
	else if (!pwd)
	{
		d->ret = _FAILURE;
		return ( _FAILURE);
	}
	else if (!(d->options & _N) && !(new->pwd->pw_name = ft_strdup(pwd->pw_name)))
	{
		free_files(&new);
		error_ls(d, strerror(errno));
	}
	return (_SUCCESS);
}

int         init_grp(t_all *d, t_files *new)
{
	struct group        *grp;

	errno = 0;
	grp = getgrgid(new->st->st_gid);
	if (errno)
	{
		ft_printf("%s\n", new->path);
		ft_dprintf(_ERR, "getgrgid: %s\n", strerror(errno));
		errno = 0;
		d->ret = _FAILURE;
		return (_FAILURE);
	}
	else if (!grp)
	{
		d->ret = _FAILURE;
		return (_FAILURE);
	}
	else if (!(d->options & _N) && !(new->grp->gr_name = ft_strdup(grp->gr_name)))
	{
		free_files(&new);
		error_ls(d, strerror(errno));
	}
	return (_SUCCESS);
}

int     init_id(t_all *d, t_files *new)
{
	if (!(new->pwd = (struct passwd *)ft_memalloc(sizeof(struct passwd))) ||
			!(new->grp = (struct group *)ft_memalloc(sizeof(struct group))))
	{
		free_files(&new);
		error_ls(d, strerror(errno));
	}
	if ((init_pwd(d, new) == _FAILURE))
		ft_memdel((void *)&new->pwd);
	if ((init_grp(d, new) == _FAILURE))
		ft_memdel((void *)&new->grp);
	return (_SUCCESS);
}

t_files     *init_files(t_all *d, char *name, char *path)
{
	t_files     *new;

	if (!(new = (t_files *)ft_memalloc(sizeof(t_files))))
		error_ls(d, strerror(errno));
	if (!(new->st = (struct stat *)ft_memalloc(sizeof(struct stat))))
	{
		free(new);
		error_ls(d, strerror(errno));
	}
	if (!(new->name = ft_strdup(name)) || (!path && !((new->path = ft_strdup(name)))))
	{
		free_files(&new);
		error_ls(d, strerror(errno));
	}
	if (path && !((new->path = ft_nstrjoin(3, path, "/", name))))
	{
		free_files(&new);
		error_ls(d, strerror(errno));
	}
	return (new);
}

t_dir       *init_dir(t_all *d, char *name)
{
	t_dir   *new;

	if (!(new = (t_dir *)ft_memalloc(sizeof(t_dir))))
		error_ls(d, strerror(errno));
	if (!(new->path = ft_strdup(name)))
	{
		free(new);
		error_ls(d, strerror(errno));
	}
	return (new);
}

