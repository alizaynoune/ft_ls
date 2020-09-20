#include "ft_ls.h"

void		_free_start(t_all *all)
{
	t_start		*ptr;

	while (all->start_dir)
	{
		ptr = all->start_dir;
		all->start_dir = all->start_dir->next;
		(ptr->st) ? ft_memdel((void *)&ptr->st) : 0;
		(ptr->name) ? ft_memdel((void *)&ptr->name) : 0;
		ft_memdel((void *)&ptr);
	}
}

void		_free_content(t_content *content)
{
	content->name ? ft_memdel((void *)&content->name) : 0;
	content->path ? ft_memdel((void *)&content->path) : 0;
	content->st ? ft_memdel((void *)&content->st) : 0;
	content->owner ? ft_memdel((void *)&content->owner) : 0;
	content->group ? ft_memdel((void *)&content->group) : 0;
	ft_memdel((void *)&content);

}

void		_free_files(t_all *all)
{
	t_content	*content;

       while (all->files->content)
	{
		content = all->files->content;
		all->files->content = all->files->content->next;
		_free_content(content);
	}
	all->files ? ft_memdel((void *)&all->files) : 0;
}

void		_free_all(t_all *all)
{	
	t_dir	*dir;
	t_content	*content;

	while (all->dir)
	{
		dir = all->dir;
		all->dir = all->dir->next;
		while (dir->content)
		{
			content = dir->content;
			dir->content = dir->content->next;
			_free_content(content);
		}
		dir->name ? ft_memdel((void *)&dir->name) : 0;
		ft_memdel((void *)&dir);
	}
	_free_start(all);
	all->files ? _free_files(all) : 0;
	ft_memdel((void *)&all);
}

void		_failed(t_all *all, char *err)
{
	all ? _free_all(all) : 0;
	ft_dprintf(1, "%s\n", err);
	exit(_TROUBLE);
}

void		_invalid(t_all *all, char op)
{
	_free_all(all);
	ft_dprintf(1, "ft_ls: invalid option -- '%c'\n", op);
	exit(_FAILURE);
}

void		_swap_nodes(t_start *frst, t_start *lst)
{
	void		*ptr;

	ptr = (void *)frst->name;
	frst->name = lst->name;
	lst->name = (char *)ptr;
	ptr = (void *)frst->st;
	frst->st = lst->st;
	lst->st = (struct stat *)ptr;
}

void		_link_dir(t_all *all, t_start *ptr)
{
	struct stat	*new;

	if (!(new = (struct stat *)malloc(sizeof(struct stat))))
		_failed(all, strerror(errno));
	if (stat(ptr->name, new))
	{
		ft_memdel((void *)&new);
		all->ret = _FAILURE;
		perror("stat");
	}
	else if ((new->st_mode & S_IFMT) == S_IFDIR)
	{
		ft_memdel((void *)&ptr->st);
		ptr->st = new;
	}
	else
		ft_memdel((void *)&new);
}

void		_sort_start(t_all *all)
{
	t_start		*frst;
	t_start		*lst;

	frst = all->start_dir;
	while(frst && !(all->options & _L))
	{
		(frst->st->st_mode & S_IFMT) == S_IFLNK ? _link_dir(all, frst) : 0;
		frst = frst->next;
	}
	frst = all->start_dir;
	while (frst)
	{
		lst = frst->next;
		while (lst)
		{
			if (all->options & _T)
			{
				if (frst->st->st_mtime < lst->st->st_mtime)
					_swap_nodes(frst, lst);
				else if (frst->st->st_mtime == lst->st->st_mtime)
					(_ls_cmp(frst->name, lst->name) > 0) ? _swap_nodes(frst, lst) : 0;
			}
			else if (_ls_cmp(frst->name, lst->name) > 0)
				_swap_nodes(frst, lst);
			lst = lst->next;
		}
		frst = frst->next;
	}
}

void		_push_start_dir(t_all *all, t_start *new)
{
	t_start		*ptr;

	ptr = all->start_dir;
	if (!all->start_dir)
		all->start_dir = new;
	else
	{
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = new;
		new->prev = ptr;
	}
}

int		_existing(t_all *all, t_start *new)
{
	if (!(new->st = (struct stat *)malloc(sizeof(struct stat))))
	{
		ft_memdel((void *)&new->name);
		ft_memdel((void *)&new);
		_failed(all, strerror(errno));
	}
	if (lstat(new->name, new->st))
	{
		ft_dprintf(1, "ft_ls: cannot access '%s': %s\n", new->name, strerror(errno));
		ft_memdel((void *)&new->name);
		ft_memdel((void *)&new->st);
		ft_memdel((void *)&new);
		all->ret = _FAILURE;
		return (_FAILURE);
	}
	return (_SUCCESS);
}

void		start_dir(char *name, t_all *all)
{
	t_start		*new;
	size_t		len;

	if (!(new = (t_start *)ft_memalloc(sizeof(t_start))))
		_failed(all, strerror(errno));
	if (!(new->name = ft_strdup(name)))
	{
		ft_memdel((void *)&new);
		_failed(all, strerror(errno));
	}
	if (_existing(all, new) == _SUCCESS)
	{
		_push_start_dir(all, new);
		len = ft_strlen(new->name);
		if (new->name[len - 1] == '/' && len > 1)
			new->name[len - 1] = 0;
	}
}

void		parse_options(char *str, t_all *all)
{
	int	i;

	i = 1;
	while (str[i])
	{
		if (str[i] == 'l')
			all->options |= _L;
		else if (str[i] == 'R')
			all->options |= R_R;
		else if (str[i] == 'r')
			all->options |= _R;
		else if (str[i] == 'a')
			all->options |= _A;
		else if (str[i] == 't')
			all->options |= _T;
		else if (str[i] == 'U')
			all->options |= _U;
		else if (str[i] == 'G')
			all->options |= _G;
		else
			_invalid(all, str[i]);
		i++;
	}
	i == 1 ? start_dir(str, all) : 0;
}

void		_options(t_all *all, char **str, int l)
{
	int	i;

	i = 0;
	while (i < l)
	{
		str[i][0] == '-' ? parse_options(str[i], all) : start_dir(str[i], all);
		i++;
	}
}


int		main(int ac, char **av)
{
	t_all	*all;
	int	ret;

	if (!(all = (t_all *)ft_memalloc(sizeof(t_all))))
		_failed(NULL, strerror(errno));
	all->ret = _SUCCESS;
	ac > 1 ? _options(all, &av[1], ac - 1) : 0;
	(all->start_dir) && !(all->options & _U) ? _sort_start(all) : 0;
	!all->start_dir && all->ret == _SUCCESS ? start_dir(".", all) : 0;
	(all->options & _U) ? all->options -= (all->options & _R) : 0;
	_loop_dir(all);
	_print_out(all);
	ret = all->ret;
	_free_all(all);
	return (ret);
}
