/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/25 19:08:22 by alzaynou          #+#    #+#             */
/*   Updated: 2020/11/24 20:46:29 by alzaynou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		extended_attribute(t_all *d, t_files *f)
{
//	acl_t       acl;

	errno = 0;
	if ((f->len_xattr = listxattr(f->path, NULL, 0)) > 0)//, XATTR_NOFOLLOW)) > 0)
		ft_printf("@");
/*	else if ((acl = acl_get_link_np(f->path, ACL_TYPE_EXTENDED)))
	{
		acl_free(acl);
		ft_printf("+");
		f->len_xattr = 0;
	}
*/	else if (errno == ENOMEM)
		error_ls(d, strerror(errno));
	else
	{
		f->len_xattr = 0;
		ft_printf(" ");
	}
	errno = 0;
}

void		print_color(t_files *f, mode_t type)
{
	(S_ISBLK(type)) ? ft_printf(C_BLK) : 0;
	(S_ISCHR(type)) ? ft_printf(C_CHR) : 0;
	(S_ISDIR(type)) ? ft_printf(C_DIR) : 0;
	(S_ISFIFO(type)) ? ft_printf(C_FIFO) : 0;
	(S_ISLNK(type)) ? ft_printf(C_LNK) : 0;
	(S_ISSOCK(type)) ? ft_printf(C_SOCK) : 0;
	if (S_ISREG(type))
		((f->st->st_mode & (S_IXUSR | S_IXGRP | S_IXOTH))) ? ft_printf(C_EXE)
			: 0;
	ft_printf("%s", f->name);
	ft_printf(C_DEF);
}

void        major_minor(t_all *d, t_files *f)
{
	if (d)
		ft_printf(" %*d, %*d ", d->len[_MAJ_MIN], major(f->st->st_rdev),
				d->len[_MAJ_MIN], minor(f->st->st_rdev));
}

void		long_format(t_all *d, t_files *f)
{
	(d->options & _I) ? ft_printf("%*d ", d->len[_INODE], f->st->st_ino) : 0;
	(d->options & _S) ? ft_printf("%*d ", d->len[_BLOCK], f->st->st_blocks) : 0;
	print_type((f->st->st_mode));
	print_permission(f->st->st_mode);
	extended_attribute(d, f);
	ft_printf(" %*d", d->len[_LINK], f->st->st_nlink);
	print_uid_grid(d, f);
	(!(S_ISCHR(f->st->st_mode)) && !(S_ISBLK(f->st->st_mode))) ?
		ft_printf(" %*lld ", d->len[_SIZE], f->st->st_size) : major_minor(d,f);
	print_time(d, f);
	(d->options & _G) ? print_color(f, f->st->st_mode) :
		ft_printf("%s", f->name);
	(f->link) ? ft_printf(" -> %s\n", f->link) : ft_printf("\n");
	((d->options & _XATT) && f->len_xattr) ? print_xattr(d, f) : 0;

}

void		print_files(t_all *d, t_files *f)
{
	if ((d->options & _L))
		long_format(d, f);
	else
	{
		(d->options & _I) ?
			ft_printf("%*d ", d->len[_INODE], f->st->st_ino) : 0;
		(d->options & _S) ?
			ft_printf("%*d ", d->len[_BLOCK], f->st->st_blocks) : 0;
		(d->options & _G) ?
			print_color(f, f->st->st_mode) : ft_printf("%s", f->name);
		ft_printf("\n");
	}
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
	if (head_wait)
	{
		last_wait->next = curr->next;
		curr->next = head_wait;
	}
}
