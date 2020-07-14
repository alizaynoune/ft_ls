#include "ft_ls.h"

t_content	*pos_content(t_struct *d, char *name)
{
	t_content	*ptr;

	ptr = d->curr->content;
	if (!ptr || ft_strcmp(ptr->dirent->d_name, name) >= 0)
		return (NULL);
	while (ptr->next)
	{
		if (ft_strcmp(ptr->next->dirent->d_name, name) >= 0)
			return (ptr);
		ptr = ptr->next;
	}
	return (ptr);
}

void		push_content(t_struct *d, t_content *new)
{
	t_content	*ptr;

	ptr = pos_content(d, new->dirent->d_name);
	if (!ptr)
	{
		new->next = d->curr->content;
		d->curr->content = new;
	}
	else
	{
		new->next = ptr->next;
		ptr->next = new;
	}
//		ft_dprintf(1, "[%s]", new->dirent->d_name);
}

void		_content(t_struct *d, struct dirent *dirent, char *parent)
{
	t_content	*new;

	if (!(new = (t_content *)ft_memalloc(sizeof(t_content))))
		_error(d, ft_strdup(strerror(errno)), _TROUBLE);
	new->dirent = dirent;
	new->parent = parent;
	push_content(d, new);
	//ft_dprintf(1, "//%s//", new->dirent->d_name);
}

void		_push_dir(t_struct *d, DIR *dir, t_dir *new)
{
	t_dir	*ptr;

	ptr = d->dir;
	if (d->dir)
	{
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = new;
	}
	else
		d->dir = new;
	d->curr = new;
}

void		_recursive(t_struct *d, t_content *content)
{
	char	*ptr;
	char	*name;

	while (content)
	{
		if (content->dirent->d_type == DT_DIR && ft_strcmp(content->dirent->d_name, ".") && ft_strcmp(content->dirent->d_name, ".."))
		{
			ptr = ft_strjoin(content->parent, "/");
			name = ft_strjoin(ptr, content->dirent->d_name);
			ft_strdel(&ptr);
			read_dir(d, name);
		}
		content = content->next;
	}
}

int		read_dir(t_struct *d, char *name)
{
	struct dirent	*dirent;
	DIR		*dir;
	t_dir		*new;

	if (dir = opendir(name))
	{
		if (!(new = (t_dir *)ft_memalloc(sizeof(t_dir))))
			_error(d, ft_strdup(strerror(errno)), _TROUBLE);
		new->n_dir = name;
		new->p_dir = dir;
		_push_dir(d, dir, new);
		while ((dirent = readdir(dir)))
			_content(d, dirent, name);
		(d->options & R_R) ? _recursive(d, new->content) : 0;
	}
	else
		_error(d, ft_nstrjoin(4, "ft_ls: cannot access '", name, "' : ", strerror(errno)), _TROUBLE);
	return(_SUCCESS);
}
