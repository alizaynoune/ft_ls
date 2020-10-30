/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/25 19:08:22 by alzaynou          #+#    #+#             */
/*   Updated: 2020/10/30 05:33:02 by alzaynou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		print_type(unsigned int type)
{
	char	*str;
	int		i;

	str = "?-bcdlps";
	i = 0;
	((type == S_IFREG)) ? i = 1 : 0;
	((type == S_IFBLK)) ? i = 2 : 0;
	((type == S_IFCHR)) ? i = 3 : 0;
	((type == S_IFDIR)) ? i = 4 : 0;
	((type == S_IFLNK)) ? i = 5 : 0;
	((type == S_IFIFO)) ? i = 6 : 0;
	((type == S_IFSOCK)) ? i = 7 : 0;
	ft_printf("%c", str[i]);
}

void		print_permission(unsigned int mode)
{
	int8_t		shift;
	int8_t		perm;

	shift = 2;
	while (shift >= 0)
	{
		perm = ((mode & (7 << (shift * 3))) >> (shift * 3));
		(perm & P_R) ? ft_printf("r") : ft_printf("-");
		(perm & P_W) ? ft_printf("w") : ft_printf("-");
		(shift && (perm & P_X)) ? ft_printf("x") : ft_printf("-");
		if (!shift)
		{
			if (!(mode & S_ISVTX))
				(perm & P_X) ? ft_printf("x") : ft_printf("-");
			else
				(perm & P_X) ? ft_printf("t") : ft_printf("T");
		}
		shift--;
	}
}

void        print_time(t_all *d, t_files *f)
{
    char    *time;

    if ((time = ctime(&f->st->st_mtime)))
        ft_printf("%.12s ", time + 4);
    else
    {
        ft_dprintf(2, "ctime: %s", strerror(errno));
        d->ret = _FAILURE;
    }
}

void		print_uid_grid(t_all *d, t_files *f)
{
	((d->options & _N)) ? ft_printf(" %-*d ", d->len[_OWNER], f->pwd->pw_uid) :
		ft_printf(" %-*s ", d->len[_OWNER], f->pwd->pw_name);
	((d->options & _N)) ? ft_printf(" %-*d ", d->len[_GROUP], f->grp->gr_gid) :
		ft_printf(" %-*s ", d->len[_GROUP], f->grp->gr_name); 

}

void		extended_attribute(t_files *f)
{
	ssize_t		attr;

	attr = listxattr(f->path, NULL, 0, XATTR_NOFOLLOW);
	((attr > 0)) ? ft_printf("@") : 0;
	((attr == -1)) ? ft_printf("+") : 0;
	((attr == 0)) ? ft_printf(" ") : 0;
//	ft_printf("[%d]", errno);
}

void		print_files(t_all *d, t_files *f)
{
	unsigned int		type;

	type = (f->st->st_mode & S_IFMT);
	if ((d->options & _L))
	{
		print_type((type));
		print_permission(f->st->st_mode);
		extended_attribute(f);
		//print extended attributes
		ft_printf(" %*d", d->len[_LINK], f->st->st_nlink);
		(f->pwd && f->grp) ? print_uid_grid(d, f) : 0;
		ft_printf("%*d ", d->len[_SIZE], f->st->st_size);
        print_time(d, f);
		ft_printf("%s", f->name);
		(f->link) ? ft_printf(" -> %s\n", f->link) : ft_printf("\n");
        // if is link print name of stat file
	}
	else
		ft_printf("%s\n", f->name);
}

t_waiting		*recursuvely(t_all *d, t_files *f, t_waiting *curr)
{
	t_waiting		*new;
	char			*n_name;

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
	while (tmp)
	{
		print_files(d, tmp);
		if ((d->options & _R_) && ((tmp->st->st_mode & S_IFMT) == S_IFDIR))
		{
			if (h_w)
				((l_w->next = recursuvely(d, tmp, curr))) ? l_w = l_w->next : 0;
			else
				((h_w = recursuvely(d, tmp, curr))) ? l_w = h_w : 0;
		}
		tmp = ((d->options & _R)) ? tmp->prev : tmp->next;
	}
	if (h_w)
	{
		l_w->next = curr->next;
		curr->next = h_w;
	}
    ft_memset((void *)d->len, 0, _MAX_LEN_TABLE);
}
