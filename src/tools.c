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

void         read_link(t_all *d, t_files *f)
{
	ssize_t     size;
	ssize_t     loop;

	if (!(S_ISLNK(f->st->st_mode)))
		return ;
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
		while (loop == _FAILURE && loop > -1)
		{
			ft_strdel(&f->link);
			loop = fix_size_link(d, f, size);
			size *= 2;
		}
		(loop < 0) ? size = loop : 0;
	}
    (size < 0 ) ? error_read_link(d, f->path) : 0;
}

int         stat_file(char *f, struct stat *st)
{
	if ((stat(f, st)) == -1)
		return (_FAILURE);
	return (_SUCCESS);
}

int         lstat_file(char *f, struct stat *st)
{
	if ((lstat(f, st)) == -1)
		return (_FAILURE);
	return (_SUCCESS);
}
