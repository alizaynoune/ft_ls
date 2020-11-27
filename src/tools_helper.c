/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 10:44:02 by alzaynou          #+#    #+#             */
/*   Updated: 2020/11/27 11:48:52 by alzaynou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

t_waiting			*recursuvely(t_all *d, t_files *f)
{
	t_waiting		*new;

	if (!ft_strcmp(f->name, ".") || !ft_strcmp(f->name, ".."))
		return (NULL);
	new = init_waiting(d, f);
	return (new);
}

void				override_active_options(t_all *d)
{
	(d->options & _N) ? d->options |= _L : 0;
	(d->options & _D) ? d->options -= (d->options & _R_) : 0;
	(d->options & _F) ? d->options -= (d->options & _R) : 0;
	(d->options & _F) ? d->options |= _A : 0;
}

void				extended_attribute(t_all *d, t_files *f)
{
	errno = 0;
	if ((f->len_xattr = listxattr(f->path, NULL, 0)) > 0)
		ft_printf("+");
	else if (errno == ENOMEM)
		error_ls(d, strerror(errno));
	else
	{
		f->len_xattr = 0;
		ft_printf(" ");
	}
	errno = 0;
}

void				major_minor(t_all *d, t_files *f)
{
	if (d)
		ft_printf("%*d, %*d ", d->len[_MAJ_MIN], major(f->st->st_rdev),
				d->len[_MAJ_MIN], minor(f->st->st_rdev));
}
