# include "ft_ls.h"






/*
void	_push_content(t_all *all, t_content *new)
{
	new->next = all->curr->content;
	all->curr->content ? all->curr->content->prev = new : 0;
	all->curr->content = new;
}

void	_more_info(t_all *all, t_content *new)
{
	if (!(new->st = (struct stat *)malloc(sizeof(struct stat))))
		_failed(all, strerror(errno));
	if (stat(new->path, new->st))
		_failed(all, strerror(errno));
}

void	_content(struct dirent *dirent, t_all *all, char *path)
{
	t_content	*new;

	if (!(new = (t_content *)ft_memalloc(sizeof(t_content))))
		_failed(all, strerror(errno));
	_push_content(all, new);
	if (!(new->dirent = (struct dirent *)malloc(sizeof(struct dirent))))
		_failed(all, strerror(errno));
	ft_memcpy(new->dirent, dirent, sizeof(struct dirent));
	if (!(new->path = ft_nstrjoin(3, path, "/", new->dirent->d_name)))
		_failed(all, strerror(errno));
	(all->options & _L) ? _more_info(all, new) : 0;
}

void	_remove(t_all *all, t_dir *dir)
{
	t_dir	*ptr;

	ptr = dir;
	if (all->dir == dir)
	{
		all->dir = all->dir->next;
		dir->next ? dir->next->prev = all->dir : 0;
		all->dir ? all->dir->prev = NULL : 0;
		ft_memdel((void *)&dir->name);
		ft_memdel((void *)&dir);
	}
	else
	{
		ptr->prev->next = ptr->next;
		ptr->next ? ptr->next->prev = ptr->prev : 0;
		ft_memdel((void *)&dir->name);
		ft_memdel((void *)&dir);
	}
}


int	_open_dir(t_all *all, t_dir *ptr)
{
	DIR		*dir;
	struct dirent	*dirent;

	if ((dir = opendir(ptr->name)))
	{
		while ((dirent = readdir(dir)))
			_content(dirent, all, ptr->name);// if failed closedir;
		closedir(dir);
	}
	// else if a file not dir
	else
	{
		ft_dprintf(1, "ft_ls: cannot access \"%s\" : %s\n", ptr->name, strerror(errno));
		all->ret = _FAILURE;
		return (_FAILURE);
	}
	return (_SUCCESS);
}

void	_loop_dir(t_all *all)
{
	t_dir	*ptr;
	t_dir	*curr;

	// you can add other sort;
	all->lst_dir = all->dir;
	curr = all->lst_dir;
	while (curr)
	{
		ptr = curr;
		all->lst_dir = curr;
		all->curr = curr;
		if ((_open_dir(all, ptr) == _FAILURE))
			_remove(all, curr);
		curr = curr->next;
	}
}*/
