#include "ft_ls.h"

t_files     *init_files(t_all *d, char *name, char *path)
{
	t_files     *new;

	if (!(new = (t_files *)ft_memalloc(sizeof(t_files))))
		error_ls(d, strerror(errno));
	if (!(new->st = (struct stat *)ft_memalloc(sizeof(struct stat))))
	{
		free(new);
		error_ls(d, strerror(errno));
	}
	if (!(new->name = ft_strdup(name)) ||
            (!path && !((new->path = ft_strdup(name)))))
	{
		free_files(&new);
		error_ls(d, strerror(errno));
	}
	if (path && !((new->path = ft_nstrjoin(3, path, "/", name))))
	{
		free_files(&new);
		error_ls(d, strerror(errno));
	}
	return (new);
}

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

t_waiting       *init_waiting(t_all *d, t_files *f)
{
	t_waiting       *new;

	if (!(new = (t_waiting *)ft_memalloc(sizeof(t_waiting))))
		error_ls(d, strerror(errno));
	new->name = f->name;
	new->full_name = f->path;
	new->st = f->st;
	f->name = NULL;
	f->path = NULL;
	f->st = NULL;
	return (new);
}
