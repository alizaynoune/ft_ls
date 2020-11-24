/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/24 20:45:32 by alzaynou          #+#    #+#             */
/*   Updated: 2020/11/24 20:50:08 by alzaynou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

t_waiting       *recursuvely(t_all *d, t_files *f)
{
	t_waiting       *new;

	if (!ft_strcmp(f->name, ".") || !ft_strcmp(f->name, ".."))
		return (NULL);
	new = init_waiting(d, f);
	return (new);
}

int         error_ls(t_all *d,  char *err)
{
	errno = 0;
	ft_dprintf(_ERR, "%s\n", err);
	if (d->fd_dir)
		((closedir(d->fd_dir) == -1)) ? ft_dprintf(_ERR, "ls: connot close dir %s",
			strerror(errno)) : 0;
	free_all(d);
	exit(_TROUBLE);
}
