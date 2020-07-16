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

	ptr = (!(d->options & _U)) ? pos_content(d, new->dirent->d_name) : d->curr->lst;
	if (!ptr)
	{
		!(d->curr->lst) ? d->curr->lst = new : 0;
		new->next = d->curr->content;
		d->curr->content = new;
	}
	else
	{
		new->next = ptr->next;
		ptr->next = new;
	}
	(d->curr->lst->next) ? d->curr->lst = d->curr->lst->next : 0;
}

void		_time(t_struct *d, char *time, t_content *new)
{
	if (!(new->time = ft_strdup(ft_strchr(time, ' '))))
		_error(d, ft_strdup(strerror(errno)), _TROUBLE);
	ft_strclr(ft_strrchr(new->time, ':'));
}

void		_list_stock(t_struct *d, t_content *new)
{
	char		*time;
	struct passwd	*pw;
	struct group	*gr;
	
	(stat(new->n_file, &new->st) == -1) ? _error(d, ft_strdup(strerror(errno)), _TROUBLE) : 0;
	!(time = ctime(&new->st.st_ctime)) ? _error(d, ft_strdup(strerror(errno)), _TROUBLE) : _time(d, time, new);
	!(pw = getpwuid(new->st.st_uid)) ? _error(d, ft_strdup(strerror(errno)), _TROUBLE) : 0;
	!(new->owner = ft_strdup(pw->pw_name)) ? _error(d, ft_strdup(strerror(errno)), _TROUBLE) : 0;
	!(gr = getgrgid(new->st.st_gid)) ? _error(d, ft_strdup(strerror(errno)), _TROUBLE) : 0;
	!(new->group = ft_strdup(gr->gr_name)) ? _error(d, ft_strdup(strerror(errno)), _TROUBLE) : 0;
}

void		_content(t_struct *d, struct dirent *dirent, char *parent)
{
	t_content	*new;

	if (dirent->d_name[0] == '.' && !(d->options & _A))
		return ;
	if (!(new = (t_content *)ft_memalloc(sizeof(t_content))))
		_error(d, ft_strdup(strerror(errno)), _TROUBLE);
	new->dirent = dirent;
	new->parent = parent;
	push_content(d, new);
	if (!(new->n_file = ft_nstrjoin(3, parent, "/", new->dirent->d_name)))
		_error(d, ft_strdup(strerror(errno)), _TROUBLE);
	(d->options & _L) ? _list_stock(d, new) : 0;
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
	while (content)
	{
		if (content->dirent->d_type == DT_DIR && ft_strcmp(content->dirent->d_name, ".") && ft_strcmp(content->dirent->d_name, ".."))
			read_dir(d, ft_nstrjoin(3 ,content->parent, "/", content->dirent->d_name));
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
