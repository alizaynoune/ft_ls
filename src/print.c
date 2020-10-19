# include "ft_ls.h"

void		_print_type(unsigned int type)
{
	if (type == S_IFREG)
		ft_dprintf(1, "-");
	else if (type == S_IFBLK)
		ft_dprintf(1, "b");
	else if (type == S_IFCHR)
		ft_dprintf(1, "c");
	else if (type == S_IFIFO)
		ft_dprintf(1, "p");
	else if (type == S_IFLNK)
		ft_dprintf(1, "l");
	else if (type == S_IFSOCK)
		ft_dprintf(1, "s");
	else if (type == S_IFDIR)
		ft_dprintf(1, "d");
	else
		ft_dprintf(1, "?");

}

void		_print_mod(unsigned int mode)
{
	int8_t	sheft;
	int8_t	perm;

	sheft = 2;
	_print_type(mode & S_IFMT);
	while (sheft >= 0)
	{
		perm = (mode & (0x7 << (sheft * 3))) >> (sheft * 3);
		(perm & R_) ? ft_dprintf(1, "r") : ft_dprintf(1, "-");
		(perm & W_) ? ft_dprintf(1, "w") : ft_dprintf(1, "-");
		if (sheft || (!sheft && !(mode & S_ISVTX)))
			(perm & _X_) ? ft_dprintf(1, "x") : ft_dprintf(1, "-");
		sheft--;
	}
	if ((mode & S_ISVTX))
		((mode & S_IFMT) == S_IFREG) && !(perm & _X_) ? ft_dprintf(1, "T") : ft_dprintf(1, "t");
}

void		_print_date(t_all *all, time_t date)
{
	char	*time;

	if ((time = ctime(&date)))
		ft_dprintf(1, " %.12s ", time + 4);
	else
	{
		perror("mtime");
		all->ret = _FAILURE;
	}
}

void		_print_color(unsigned int mode, char *name, size_t len)
{
	unsigned int type;

	type = mode & S_IFMT;
	if (type == S_IFBLK)
		ft_dprintf(1, "%s", C_BLK);
	else if (type == S_IFCHR)
		ft_dprintf(1, "%s", C_CHR);
	else if (type == S_IFIFO)
		ft_dprintf(1, "%s", C_FIFO);
	else if (type == S_IFLNK)
		ft_dprintf(1, "%s", C_LNK);
	else if (type == S_IFSOCK)
		ft_dprintf(1, "%s", C_SOCK);
	else if (type == S_IFDIR)
		ft_dprintf(1, "%s", C_DIR);
	else if (type == S_IFREG)
		((mode & S_IXUSR) && (mode & S_IXGRP) && (mode & S_IXOTH)) ? ft_dprintf(1, "%s", C_EXE) : 0;
	ft_dprintf(1, "%-*s%s", len, name, C_DEF);
}

void		_is_link(t_all *all, char *path, struct stat *st)
{
	char	*name;

	if (!(name = (char *)malloc((sizeof(char)) * st->st_size + 1)))
		_failed(all, strerror(errno));
	if ((readlink(path, name, st->st_size + 1)) < 0)
	{
		perror("readlink");
		all->ret = _FAILURE;
	}
	name[st->st_size] = 0;
	if (!(all->options & _G))
		ft_dprintf(1, " -> %s", name);
	else
	{
		if (stat(name, st))
			all->ret = _FAILURE;
		ft_dprintf(1, " -> ");
		_print_color(st->st_mode, name, 0);
	}
	ft_memdel((void *)&name);
}

void		_print_long(t_all *all, t_dir *dir, t_content *ptr)
{
	_print_mod(ptr->st->st_mode);
	ft_dprintf(1, " %*lu ", dir->len[LINK_], ptr->st->st_nlink);
	ft_dprintf(1, "%-*s %-*s", dir->len[OWNER_], ptr->owner, dir->len[GROUP_], ptr->group);
	ft_dprintf(1, " %*ld", dir->len[SIZE_], ptr->st->st_size);
	_print_date(all, ptr->st->st_mtime);
	(all->options & _G) ? _print_color(ptr->st->st_mode, ptr->name, dir->len[NAME_]) : ft_dprintf(1, " %s", ptr->name);
	((ptr->st->st_mode & S_IFMT) == S_IFLNK) ? _is_link(all, ptr->path, ptr->st) : 0;
	ft_dprintf(1, "\n");
}

void		_print_simple(t_all *all, t_content *ptr, size_t len)
{
	(all->options & _G) ? _print_color(ptr->st->st_mode, ptr->name, len) : ft_dprintf(1, "[%-*s]", len + 2, ptr->name);
	//ft_dprintf(1, "  ");
}

void		_print_out(t_all *all)
{
	t_dir		*dir;
	t_content	*content;

	content = NULL;
	if (all->files)
		content = (all->options & _R) ? all->files->lst_content : all->files->content;
	while (content)
	{
		(all->options & _L) ? _print_long(all, all->files, content) : _print_simple(all, content, all->files->len[NAME_]);
		content = (all->options & _R) ? content->prev : content->next;
	}
	!(all->options & _L) && all->files ? ft_dprintf(1, "\n") : 0;
	dir = (all->options & _R)  ? all->lst_dir : all->dir;
	while (dir)
	{
		(all->dir->next || all->files) ? ft_dprintf(1, "\n%s:\n", dir->name) : 0;
		(all->options & _L) ? ft_dprintf(1, "total %llu\n", dir->total) : 0;
		content = (all->options & _R) ? dir->lst_content : dir->content;
		while (content)
		{
			(all->options & _L) ? _print_long(all, dir, content) : _print_simple(all, content, dir->len[NAME_]);
			content = (all->options & _R) ? content->prev : content->next;
		}
		!(all->options & _L) ? ft_dprintf(1, "\n") : 0;
		dir = (all->options & _R) ? dir->prev : dir->next;
	}
}
