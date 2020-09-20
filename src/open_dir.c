# include "ft_ls.h"

void		_push_dir(t_all *all, t_dir *new)
{
	if (!all->dir)
	{
		all->dir = new;
		all->lst_dir = new;
	}
	else
	{
		all->lst_dir->next = new;
		new->prev = all->lst_dir;
		all->lst_dir = new;
	}
}


void		_sort_content_time(t_all *all, t_content *new)
{
	t_content	*ptr;

	ptr = all->lst_dir->content;
	if (!ptr)
	{
		all->lst_dir->content = new;
		all->lst_dir->lst_content = new;
		return ;
	}
	while (ptr)
	{
		if (ptr->st->st_mtime <= new->st->st_mtime)
		{
			if ((ptr->st->st_mtime == new->st->st_mtime) && (_ls_cmp(ptr->name, new->name)) <= 0)
			{
				(ptr->next) ? ptr->next->prev = new : 0;
				new->next = ptr->next;
				ptr->next = new;
				new->prev = ptr;
				ptr == all->lst_dir->lst_content ? all->lst_dir->lst_content = new : 0;
			}
			else
			{
				ptr->prev ? ptr->prev->next = new : 0;
				new->prev = ptr->prev;
				new->next = ptr;
				ptr->prev = new;
				ptr == all->lst_dir->content ? all->lst_dir->content = new : 0;
			}
			break;
		}
		ptr = ptr->next;
	}
	if (!ptr)
	{
		new->prev = all->lst_dir->lst_content;
		all->lst_dir->lst_content->next = new;
		all->lst_dir->lst_content = new;
	}
}

void		_sort_content_alpha(t_all *all, t_content *new)
{
	t_content	*ptr;

	ptr = all->lst_dir->content;
	if (!ptr)
	{
		all->lst_dir->content = new;
		all->lst_dir->lst_content = new;
		return ;
	}
	while (ptr)
	{
		if (_ls_cmp(ptr->name, new->name) >= 0)
		{
			(ptr->prev) ? ptr->prev->next = new : 0;
			new->prev = ptr->prev;
			new->next = ptr;
			ptr->prev = new;
			ptr == all->lst_dir->content ? all->lst_dir->content = new : 0;
			break ;
		}
		ptr = ptr->next;
	}
	if (!ptr)
	{
		new->prev = all->lst_dir->lst_content;
		all->lst_dir->lst_content->next = new;
		all->lst_dir->lst_content = new;
	}
}

void		_get_lens(t_dir *dir, t_content *new)
{
	size_t		len[4];
	size_t		loop;

	loop = -1;
	len[LINK_] = ft_intlen(new->st->st_nlink);
	len[OWNER_] = new->owner ? ft_strlen(new->owner) : 0;
	len[GROUP_] = new->group ? ft_strlen(new->group) : 0;
	len[SIZE_] = ft_intlen(new->st->st_size);
	while (++loop < 4)
		dir->len[loop] < len[loop] ? dir->len[loop] = len[loop] : loop;
}

void		_push_content(t_all *all, t_content *new)
{
	size_t	len;
	if (all->options & _U)
	{
		if (!all->lst_dir->content)
		{
			all->lst_dir->content = new;
			all->lst_dir->lst_content = new;
		}
		else
		{
			all->lst_dir->lst_content->next = new;
			new->prev = all->lst_dir->lst_content;
			all->lst_dir->lst_content = new;
		}
	}
	else if (all->options & _T)
		_sort_content_time(all, new);
	else
		_sort_content_alpha(all, new);
	if (!(all->options & _L))
	{
		len = ft_strlen(new->name);
		(all->lst_dir->len[NAME_] < len) ? all->lst_dir->len[NAME_] = len : 0;
	}
}

void		_more_info(t_all *all, t_content *new, t_dir *dir, DIR *id_dir)
{
	struct passwd	*pwd;
	struct group	*group;

	group = NULL;
	pwd = NULL;
	dir->total += (new->st->st_blocks / 2);
	if (!(pwd = getpwuid(new->st->st_uid)))
	{
		perror("\ngetpwuid");
		all->ret = _FAILURE;
	}
	if (pwd && !(new->owner = ft_strdup(pwd->pw_name)))
	{
		(id_dir) ? closedir(id_dir) : 0;
		_free_content(new);
		_failed(all, strerror(errno));
	}
	if (!(group = getgrgid(new->st->st_gid)))
	{
		perror("\ngetgrgid");
		all->ret = _FAILURE;
	}
	if (group && !(new->group = ft_strdup(group->gr_name)))
	{
		(id_dir) ? closedir(id_dir) : 0;
		_free_content(new);
		_failed(all, strerror(errno));
	}
	_get_lens(dir, new);
}

t_content	*_alloc_content(t_all *all, struct dirent *dirent, char *path, DIR *id_dir)
{
	t_content	*new;

	if (!(all->options & _A) && dirent->d_name[0] == '.')
		return (NULL);
	if (!(new = (t_content *)ft_memalloc(sizeof(t_content))))
	{
		closedir(id_dir);
		_failed(all, strerror(errno));
	}
	(path[0] == '/') ? path[0] = 0 : 0;
	if (!(new->name = ft_strdup(dirent->d_name)) || !(new->path = ft_nstrjoin(3, path, "/", new->name))
			|| !(new->st = (struct stat *)malloc(sizeof(struct stat))))
	{
		closedir(id_dir);
		_free_content(new);
		_failed(all, strerror(errno));
	}
	if (lstat(new->path, new->st))
	{
		perror("lstat");
		_free_content(new);
		all->ret = _FAILURE;
	}
	(all->options & _L) ? _more_info(all, new, all->lst_dir, id_dir) : 0;
	return (new);
}

void		_read_dir(t_all *all, DIR *id_dir, char *name)
{
	t_dir		*new;
	struct dirent	*dirent;
	t_content	*content;

	if (!(new = (t_dir *)ft_memalloc(sizeof(t_dir))))
	{
		closedir(id_dir);
		_failed(all, strerror(errno));
	}
	if (!(new->name = ft_strdup(name)))
	{
		free(new);
		closedir(id_dir);
		_failed(all, strerror(errno));
	}
	_push_dir(all, new);
	while ((dirent = readdir(id_dir)))
		(content = _alloc_content(all, dirent, name, id_dir)) ? _push_content(all, content) : 0;
	closedir(id_dir);
}

void		_open_dir(t_all *all, char *name)
{
	DIR	*id_dir;

	if (!(id_dir = opendir(name)))
	{
		ft_dprintf(1, "ft_ls: cannot open directory '%s': %s\n", name, strerror(errno));
		all->ret = _FAILURE;
	}
	else
	{
		_read_dir(all, id_dir, name);
		(all->options & R_R) ? _loop_dir(all) : 0;
	}
}

void		_push_files(t_all *all, t_start *ptr)
{
	t_content	*new;
	size_t		len;

	if (!all->files)
		!(all->files = (t_dir *)ft_memalloc(sizeof(t_dir))) ? _failed(all, strerror(errno)) : 0;
	if (!(new = (t_content *)ft_memalloc(sizeof(t_content))))
		_failed(all, strerror(errno));
	new->st = ptr->st;
	new->path = ptr->name;
	new->name = ft_strdup(ptr->name);
	ptr->name = NULL;
	ptr->st = NULL;
	if (all->files->content)
	{
		all->files->lst_content->next = new;
		new->prev = all->files->lst_content;
		all->files->lst_content = new;
	}
	else
	{
		all->files->content = new;
		all->files->lst_content = new;
	}
	if (!(all->options & _L))
	{
		len = ft_strlen(new->name);
		(all->files->len[NAME_] < len) ? all->files->len[NAME_] = len : 0;
	}
	(all->options & _L) ? _more_info(all, new, all->files, NULL) : 0;

}

void		_loop_dir(t_all *all)
{
	t_start		*loop;
	t_content	*content;

	if (!all->lst_dir)
	{
		loop = all->start_dir;
		while (loop)
		{
			((loop->st->st_mode & S_IFMT) == S_IFDIR) ? _open_dir(all, loop->name) : _push_files(all, loop);
			loop = loop->next;
		}
	}
	else if (all->lst_dir)
	{
		content = all->lst_dir->content;
		while (content)
		{
			((content->st->st_mode & S_IFMT) == S_IFDIR) && (_ls_cmp(content->name, "."))
				&& (_ls_cmp(content->name, "..")) ? _open_dir(all, content->path) : 0;
			content = content->next;
		}
	}
}
