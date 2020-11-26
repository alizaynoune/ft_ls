#include "ft_ls.h"

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
