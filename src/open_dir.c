#include "ft_ls.h"

t_dir       *init_dir(t_all *d, char *name)
{
	t_dir   *new;

	if (!(new = (t_dir *)ft_memalloc(sizeof(t_dir))))
		error_ls(d, strerror(errno));
	if (!(new->path = ft_strdup(name)))
	{
		free(new);
		error_ls(d, strerror(errno));
	}
	return (new);
}

void		parsing_read_file(t_all *d, char *path, char *name)
{
	t_files		*new;
	char		*full_name;

	if (!(d->options & _A) && name && name[0] == '.')
		return ;
	if ((path[0] == '/' && !path[1]))
		path[0] = 0;
	if (!(full_name = ft_nstrjoin(3, path, "/", name)))
		error_ls(d, strerror(errno));
	new = init_files(d, name);
	if ((lstat_file(d, full_name, new->st) == _SUCCESS))
	{
		push_files(d, new, &d->dir->h_files, &d->dir->l_files);
		((new->st->st_mode & S_IFMT) == S_IFDIR) ? new->path = full_name : free(full_name);
	}
//	else if (!(d->options & _L) && (stat_file(d, full_name, new->st) == _SUCCESS))
//		push_files(d, new, &d->dir->h_files, &d->dir->l_files);
	else
	{
		free_files(new);
		free(full_name);
	}
}

void        read_dir(t_all *d, DIR *d_dir)
{
	struct dirent   *dirent;

	errno = 0;
	while ((dirent = readdir(d_dir)))
		parsing_read_file(d, d->dir->path, dirent->d_name);
	if (errno)
	{
		ft_dprintf(2, "ls: cannot read directory%s\n", strerror(errno));
		d->ret = _FAILURE;
		errno = 0;
	}
}

void        loop_dir(t_all *d)
{
	t_waiting       *tmp;
	DIR             *d_dir;

	tmp = d->head_waiting;
	while (tmp)
	{
		if ((d_dir = opendir(tmp->name)))
		{
			d->dir = init_dir(d, tmp->name);
			read_dir(d, d_dir);
			if (closedir(d_dir) == -1)
				ft_dprintf(2, "ls: cannot close dir%s\n", strerror(errno));
			ft_printf("\n%s:\n", d->dir->path);
			loop_print_files(d, d->dir->h_files, tmp);
			free_dir(d->dir);
			d->dir = NULL;

		}
		else
		{
			ft_dprintf(2, "ls: cannot open directory '%s/': %s\n",
					tmp->name, strerror(errno));
			d->ret = _FAILURE;
		}
		tmp = tmp->next;
	}
}
