/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 10:43:00 by alzaynou          #+#    #+#             */
/*   Updated: 2020/11/27 11:00:32 by alzaynou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		print_type(mode_t type)
{
	char	*str;
	int		i;

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

void		print_permission(mode_t mode)
{
	int8_t		shift;
	int8_t		perm;

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

void		print_time(t_all *d, t_files *f)
{
	char		*str_time;
	time_t		tm;
	time_t		set_time;

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

void		long_format(t_all *d, t_files *f)
{
	(d->options & _I) ? ft_printf("%*lu ", d->len[_INODE], f->st->st_ino) : 0;
	(d->options & _S) ?ft_printf("%*lu ", d->len[_BLOCK], f->st->st_blocks) : 0;
	print_type((f->st->st_mode));
	print_permission(f->st->st_mode);
	extended_attribute(d, f);
	ft_printf(" %*ld", d->len[_LINK], f->st->st_nlink);
	print_uid_grid(d, f);
	(!(S_ISCHR(f->st->st_mode)) && !(S_ISBLK(f->st->st_mode))) ?
		ft_printf(" %*ld ", d->len[_SIZE], f->st->st_size) : major_minor(d, f);
	print_time(d, f);
	(d->options & _G) ? print_color(f, f->st->st_mode) :
		ft_printf("%s", f->name);
	(f->link) ? ft_printf(" -> %s\n", f->link) : ft_printf("\n");
	((d->options & _XATT) && f->len_xattr) ? print_xattr(d, f) : 0;
}

void		loop_print_files(t_all *d, t_files *lst, t_files *l_lst,
		t_waiting *curr)
{
	t_files		*tmp;
	t_waiting	*head_wait;
	t_waiting	*last_wait;

	head_wait = NULL;
	last_wait = NULL;
	tmp = ((d->options & _R)) ? l_lst : lst;
	(d->len[_MAJ_MIN]) ? fix_len_maj_size(d) : 0;
	while (tmp)
	{
		print_files(d, tmp);
		if ((d->options & _R_) && (S_ISDIR(tmp->st->st_mode)))
		{
			if (head_wait)
			{
				if ((last_wait->next = recursuvely(d, tmp)))
					last_wait = last_wait->next;
			}
			else if ((head_wait = recursuvely(d, tmp)))
				last_wait = head_wait;
		}
		tmp = ((d->options & _R)) ? tmp->prev : tmp->next;
	}
	(head_wait) ? push_recursuvely(last_wait, head_wait, curr) : 0;
}
