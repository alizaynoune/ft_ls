/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   links.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/24 19:38:32 by alzaynou          #+#    #+#             */
/*   Updated: 2020/11/24 20:43:16 by alzaynou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

ssize_t         fix_size_link(t_all *d, t_files *f, ssize_t size)
{
	ssize_t     len;

	errno = 0;
	if (!(f->link = (char *)ft_memalloc(sizeof(char) * size + 1)))
	{
		free_files(&f);
		error_ls(d, strerror(errno));
	}
	len = readlink(f->path, f->link, size + 1);
	if (len > size)
		return (_FAILURE);
	else if (len < 0)
		return (len);
	else
		return (_SUCCESS);
}

int         read_link(t_all *d, t_files *f)
{
	ssize_t     size;
	ssize_t     loop;

	if (!(S_ISLNK(f->st->st_mode)))
		return (_SUCCESS);
	if (!(f->link = (char *)ft_memalloc(sizeof(char) * (f->st->st_size + 1))))
	{
		free_files(&f);
		error_ls(d, strerror(errno));
	}
	size = readlink(f->path, f->link, f->st->st_size + 1);
	if (size > f->st->st_size)
	{
		size = (size) ? size * 2 : 2;
		loop = _FAILURE;
		while (loop == _FAILURE)
		{
			ft_strdel(&f->link);
			loop = fix_size_link(d, f, size);
			if (loop < 0)
				break ;
			size *= 2;
		}
		(loop < 0) ? size = loop : 0;
	}
	if (size < 0 )
	{
		ft_dprintf(_ERR, "readlink: '%s'  %s\n", strerror(errno), f->path);
		d->ret = _FAILURE;
		errno = 0;
	}
	return (_SUCCESS);
}

