/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/24 19:48:22 by alzaynou          #+#    #+#             */
/*   Updated: 2020/11/24 20:44:14 by alzaynou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void        print_type(mode_t type)
{
	char    *str;
	int     i;

	str = "?-bcdlps";
	i = 0;
	(S_ISREG(type)) ? i = 1 : 0;
	(S_ISBLK(type)) ? i = 2 : 0;
	(S_ISCHR(type)) ? i = 3 : 0;
	(S_ISDIR(type)) ? i = 4 : 0;
	(S_ISLNK(type)) ? i = 5 : 0;
	(S_ISFIFO(type)) ? i = 6 : 0;
	(S_ISSOCK(type)) ? i = 7 : 0;
	ft_printf("%c", str[i]);
}

void        print_permission(mode_t mode)
{
	int8_t      shift;
	int8_t      perm;

	shift = 2;
	while (shift >= 0)
	{
		perm = ((mode & (7 << (shift * 3))) >> (shift * 3));
		(perm & P_R) ? ft_printf("r") : ft_printf("-");
		(perm & P_W) ? ft_printf("w") : ft_printf("-");
		if (shift == 2 && (mode & S_ISGID))
			(perm & P_X) ? ft_printf("s") : ft_printf("S");
		else if (shift == 2)
			(perm & P_X) ? ft_printf("x") : ft_printf("-");
		else if (shift == 1 && (mode & S_ISUID))
			(perm & P_X) ? ft_printf("s") : ft_printf("S");
		else if (shift == 1)
			(perm & P_X) ? ft_printf("x") : ft_printf("-");
		else if (!shift && (mode & S_ISVTX))
			(perm & P_X) ? ft_printf("t") : ft_printf("T");
		else
			(perm & P_X) ? ft_printf("x") : ft_printf("-");
		shift--;
	}
}

void        print_time(t_all *d, t_files *f)
{
	char        *str_time;
	time_t      tm;
	time_t      set_time;

	errno = 0;
	tm = time(NULL);
	set_time = (d->options & _U) ? f->st->st_atime : f->st->st_mtime;
	if (!(tm == -1 && errno) && (str_time = ctime(&set_time)))
	{
		tm -= set_time;
		if (d->options & _T_)
			ft_printf("%.20s ", str_time + 4);
		else if (ft_abs(tm) < _MONTHS_)
			ft_printf("%.12s ", str_time + 4);
		else
		{
			ft_printf("%.6s  %.4s ", str_time + 4, str_time + 20);
		}
	}
	else
	{
		ft_dprintf(2, "ctime: %s", strerror(errno));
		d->ret = _FAILURE;
	}
}

void        print_uid_grid(t_all *d, t_files *f)
{
	if ((d->options & _N) || !f->pwd)
		ft_printf(" %-*d ", d->len[_OWNER], f->st->st_uid);
	else
		ft_printf(" %-*s ", d->len[_OWNER], f->pwd->pw_name);
	if ((d->options & _N) || !f->grp)
		ft_printf(" %-*d ", d->len[_GROUP], f->st->st_gid);
	else
		ft_printf(" %-*s ", d->len[_GROUP], f->grp->gr_name);
}

void        print_xattr(t_all *d, t_files *f)
{
	char        *buff;
	ssize_t     read;
	ssize_t     size;
	ssize_t     len;

	len = 0;
	read = 0;
	buff = NULL;
	if (!(buff = (char *)ft_memalloc(sizeof(char) * (f->len_xattr))))
		error_ls(d, strerror(errno));
	listxattr(f->path, buff, f->len_xattr); //, XATTR_NOFOLLOW);
	while (len < f->len_xattr)
	{
		len += ft_printf("\t%s", buff + len);
		size = getxattr(f->path, buff + read, NULL, 0);//, 0,  0);
		ft_printf("\t  %lld\n", size);
		read += len;
	}
	ft_strdel(&buff);

}
