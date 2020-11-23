/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/25 19:08:22 by alzaynou          #+#    #+#             */
/*   Updated: 2020/11/23 18:41:28 by alzaynou         ###   ########.fr       */
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

void        print_time(t_all *d, t_files *f)
{
    char    *str_time;
    time_t  tm;
    time_t  set_time;

    errno = 0;
    tm = time(NULL);
    set_time = (d->options & _U) ? f->st->st_atime : f->st->st_mtime;
    if (!(tm == -1 && errno) && (str_time = ctime(&set_time)))
    {
        tm -= set_time;
        if (d->options & _T_)
            ft_printf("%.20s ", str_time + 4);
        else if (ABS(tm) < _MONTHS_)
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

void		print_uid_grid(t_all *d, t_files *f)
{
	((d->options & _N) || !f->pwd) ? ft_printf(" %-*d ", d->len[_OWNER], f->st->st_uid)
			: ft_printf(" %-*s ", d->len[_OWNER], f->pwd->pw_name);
	((d->options & _N) || !f->grp) ? ft_printf(" %-*d ", d->len[_GROUP], f->st->st_gid) :
        ft_printf(" %-*s ", d->len[_GROUP], f->grp->gr_name); 

}

void		print_xattr(t_all *d, t_files *f)
{
	char		*buff;
	ssize_t		read;
	ssize_t		size;
	ssize_t		len;

	len = 0;
	read = 0;
	buff = NULL;
	if (!(buff = (char *)ft_memalloc(sizeof(char) * (f->len_xattr))))
		error_ls(d, strerror(errno));
	listxattr(f->path, buff, f->len_xattr, XATTR_NOFOLLOW);
	while (len < f->len_xattr)
	{
		len += ft_printf("\t%s", buff + len);
		size = getxattr(f->path, buff + read, NULL, 0, 0,  0);
		ft_printf("\t  %lld\n", size);
		read += len;
	}
	ft_strdel(&buff);

}

void		extended_attribute(t_all *d, t_files *f)
{
	acl_t       acl;
    
	errno = 0;
    if ((f->len_xattr = listxattr(f->path, NULL, 0, XATTR_NOFOLLOW)) > 0)
        ft_printf("@");
	else if ((acl = acl_get_link_np(f->path, ACL_TYPE_EXTENDED)))
    {
		acl_free(acl);
        ft_printf("+");
		f->len_xattr = 0;
    }
	else if (errno == ENOMEM)
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
        ((f->st->st_mode & (S_IXUSR | S_IXGRP | S_IXOTH))) ? ft_printf(C_EXE) : 0;
    ft_printf("%s", f->name);
    ft_printf(C_DEF);
}

void        major_minor(t_all *d, t_files *f)
{
    if (d)
		ft_printf(" %*d, %*d ", d->len[_MAJ_MIN], major(f->st->st_rdev),
				d->len[_MAJ_MIN], minor(f->st->st_rdev));
}

void		print_files(t_all *d, t_files *f)
{
    if ((d->options & _L))
    {
		(d->options & _I) ? ft_printf("%*d ", d->len[_INODE], f->st->st_ino) : 0;
        (d->options & _S) ? ft_printf("%*d ", d->len[_BLOCK], f->st->st_blocks) : 0;
        print_type((f->st->st_mode));
        print_permission(f->st->st_mode);
        extended_attribute(d, f);
        ft_printf(" %*d", d->len[_LINK], f->st->st_nlink);
        print_uid_grid(d, f);
        (!(S_ISCHR(f->st->st_mode)) && !(S_ISBLK(f->st->st_mode))) ? ft_printf(" %*lld ", d->len[_SIZE], f->st->st_size) : major_minor(d,f);
        print_time(d, f);
        (d->options & _G) ? print_color(f, f->st->st_mode) : ft_printf("%s", f->name);
        (f->link) ? ft_printf(" -> %s\n", f->link) : ft_printf("\n");
		((d->options & _XATT) && f->len_xattr) ? print_xattr(d, f) : 0;
    }
    else
    {
        (d->options & _I) ? ft_printf("%*d ", d->len[_INODE], f->st->st_ino) : 0;
        (d->options & _S) ? ft_printf("%*d ", d->len[_BLOCK], f->st->st_blocks) : 0;
        (d->options & _G) ? print_color(f, f->st->st_mode) : ft_printf("%s", f->name);
        ft_printf("\n");
    }
}

t_waiting		*recursuvely(t_all *d, t_files *f)
{
    t_waiting		*new;

    if (!ft_strcmp(f->name, ".") || !ft_strcmp(f->name, ".."))
        return (NULL);
    new = init_waiting(d, f);
    return (new);
}

void		loop_print_files(t_all *d, t_files *lst, t_files *l_lst, t_waiting *curr)
{
    t_files		*tmp;
    t_waiting	*h_w;
    t_waiting	*l_w;

    h_w = NULL;
    l_w = NULL;
    tmp = ((d->options & _R)) ? l_lst : lst;
	(d->len[_MAJ_MIN]) ? fix_len_maj_size(d) : 0;
	while (tmp)
    {
        print_files(d, tmp);
        if ((d->options & _R_) && (S_ISDIR(tmp->st->st_mode)))
        {
            if (h_w)
                ((l_w->next = recursuvely(d, tmp))) ? l_w = l_w->next : 0;
            else
                ((h_w = recursuvely(d, tmp))) ? l_w = h_w : 0;
        }
        tmp = ((d->options & _R)) ? tmp->prev : tmp->next;
    }
    if (h_w)
    {
        l_w->next = curr->next;
        curr->next = h_w;
    }
}
