#include "ft_ls.h"

void        parsing_files(t_all *d, char *f, t_files **lst, t_files **l_lst)
{
	t_files         *file;

	file = init_files(d, f, NULL);
	if ((lstat_file(f, file->st) == _SUCCESS))
	{
		(d->options & _L) ? init_id(d, file) : 0;
		(S_ISLNK(file->st->st_mode)) ? parsing_link_arg(d, file) : 0;
		push_files(d, file, lst, l_lst);
		(!(S_ISDIR(file->st->st_mode))) ? get_lens(d, file) : 0;
	}
	else
	{
		ft_dprintf(_ERR, "ls : %s: %s\n", f, strerror(errno));
		errno = 0;
		d->ret = _FAILURE;
		free_files(&file);
		d->print_path = _TROUBLE;
	}
}

void        parsing_link_arg(t_all *d, t_files *f)
{
	struct stat     *st;

	read_link(d, f);
	if (d->options & _L)
		return ;
	if (!(st = (struct stat *)ft_memalloc(sizeof(struct stat))))
	{
		free_files(&f);
		error_ls(d, strerror(errno));
	}
	if (lstat_file(f->link, st) == _SUCCESS)
	{
		ft_memdel((void *)&f->st);
		f->st = st;
	}
	else
	{
		ft_memdel((void *)&st);
	}
}

void        stat_file_reading(t_all *d, t_files *new)
{
    push_files(d, new, &d->dir->h_files, &d->dir->l_files);
    ((d->options & _L || d->options & _S)) ?
        d->dir->total += new->st->st_blocks : 0;
    ((d->options & _R_) && (S_ISDIR(new->st->st_mode))) ?
        d->print_path = _SUCCESS : 0;
    ((d->options & _L)) ? get_lens(d, new) : 0;
    (!(d->options & _L) && (d->options & _S)) ? get_len_block(d, new) : 0;
    (!(d->options & _L)) && (d->options & _I) ? get_len_inode(d, new) : 0;
}

void		parsing_read_file(t_all *d, char *path, char *name)
{
    t_files		*new;

    if (!(d->options & _A) && name && name[0] == '.')
        return ;
    if ((path[0] == '/' && !path[1]))
        path[0] = 0;
    new = init_files(d, name, path);
    if ((lstat_file(new->path, new->st) == _SUCCESS))
    {
        if ((d->options & _L))
        {
            init_id(d, new);
            read_link(d, new);
        }
        stat_file_reading(d, new);
    }
    else
    {
        ft_dprintf(_ERR, "ls: %s: %s\n", new->name, strerror(errno));
        errno = 0;
        free_files(&new);
        d->ret = _FAILURE;
    }
}

void		parsing_dir(t_all *d)
{
	t_files		*tmp;
    size_t      len;

	tmp = ((d->options & _R)) ?  d->l_arg_file : d->arg_file;
	(d->len[_MAJ_MIN]) ? fix_len_maj_size(d) : 0;
	while (tmp)
	{
		if (!(d->options & _D) && (S_ISDIR(tmp->st->st_mode)))
        {
            len = ft_strlen(tmp->path);
            (tmp->path[len - 1] == '/') && len > 1 ? tmp->path[len - 1] = 0 : 0;
            push_waiting(d, tmp);
        }
		else
		{
			d->print_path = _SUCCESS;
			print_files(d, tmp);
		}

		tmp = ((d->options & _R)) ? tmp->prev : tmp->next;
	}
	free_files(&d->arg_file);
	(d->head_waiting && (d->print_path == _SUCCESS)) ? ft_printf("\n") : 0;
	(d->print_path == _TROUBLE) ? d->print_path = _SUCCESS : 0;
}
