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

	if (!(d->options & (_A | _F)) && name && name[0] == '.')
		return ;
	if ((path[0] == '/' && !path[1]))
		path[0] = 0;
	new = init_files(d, name, path);
	if ((lstat_file(d, new->path, new->st) == _SUCCESS))
	{
		if ((d->options & _L))
        {
            if (init_id(d, new) == _FAILURE || read_link(d, new) == _FAILURE)
                return ;
        }
		push_files(d, new, &d->dir->h_files, &d->dir->l_files);
		((d->options & _L)) ? d->dir->total += new->st->st_blocks : 0;
		((d->options & _R_) && (S_ISDIR(new->st->st_mode))) ?
			d->print_path = _SUCCESS : 0;
		((d->options & _L)) ? get_lens(d, new) : 0;
		(!(d->options & _L) && (d->options & _S)) ? get_len_block(d, new) : 0;
        (!(d->options & _L)) && (d->options & _I) ? get_len_inode(d, new) : 0;
	}
	else
		free_files(&new);
}

void        read_dir(t_all *d, DIR *d_dir)
{
	struct dirent   *dirent;

	errno = 0;
	while ((dirent = readdir(d_dir)))
		parsing_read_file(d, d->dir->path, dirent->d_name);
	if (errno)
	{
		ft_dprintf(_ERR, "ls: cannot read directory '%s' %s\n", d->dir->path, strerror(errno));
		d->ret = _FAILURE;
		errno = 0;
	}
}

void        loop_dir(t_all *d)
{
	t_waiting       *tmp;

	tmp = d->head_waiting;
	(tmp && tmp->next) ? d->print_path = _SUCCESS : 0;
	while (tmp)
	{
		ft_bzero((void *)d->len, sizeof(d->len));
		(d->print_path == _SUCCESS) ? ft_printf("%s:\n", tmp->full_name) : 0;
		if ((d->fd_dir = opendir(tmp->full_name)))
		{
			d->dir = init_dir(d, tmp->full_name);
			read_dir(d, d->fd_dir);
			if (closedir(d->fd_dir) == -1)
				ft_dprintf(_ERR, "ls: cannot close dir %s\n", strerror(errno));
			((d->options & _L) && d->dir->h_files) ? ft_printf("total %d\n", d->dir->total) : 0;
			loop_print_files(d, d->dir->h_files, d->dir->l_files, tmp);
			free_dir(&d->dir);
		}
		else if (errno == ENOMEM)
			error_ls(d, strerror(errno));
		else
		{
			ft_dprintf(_ERR, "ls: cannot open directory '%s': %s\n",
					tmp->name, strerror(errno));
			d->ret = _FAILURE;
		}
		tmp = tmp->next;
		(tmp) ? ft_printf("\n") : 0;
	}
}
