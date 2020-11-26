#include "ft_ls.h"

int     init_pwd(t_all *d, t_files *new)
{
	struct passwd       *pwd;

	errno = 0;
	pwd = getpwuid(new->st->st_uid);
	if (errno)
	{
		ft_dprintf(_ERR, "getpwuid: %s", strerror(errno));
		errno = 0;
		d->ret = _FAILURE;
		return (_FAILURE);
	}
	else if (!pwd)
	{
		d->ret = _FAILURE;
		return ( _FAILURE);
	}
	else if (!(d->options & _N) &&
            !(new->pwd->pw_name = ft_strdup(pwd->pw_name)))
	{
		free_files(&new);
		error_ls(d, strerror(errno));
	}
	return (_SUCCESS);
}

int         init_grp(t_all *d, t_files *new)
{
	struct group        *grp;

	errno = 0;
	grp = getgrgid(new->st->st_gid);
	if (errno)
	{
		ft_printf("%s\n", new->path);
		ft_dprintf(_ERR, "getgrgid: %s\n", strerror(errno));
		errno = 0;
		d->ret = _FAILURE;
		return (_FAILURE);
	}
	else if (!grp)
	{
		d->ret = _FAILURE;
		return (_FAILURE);
	}
	else if (!(d->options & _N) &&
            !(new->grp->gr_name = ft_strdup(grp->gr_name)))
	{
		free_files(&new);
		error_ls(d, strerror(errno));
	}
	return (_SUCCESS);
}

int     init_id(t_all *d, t_files *new)
{
	if (!(new->pwd = (struct passwd *)ft_memalloc(sizeof(struct passwd))) ||
			!(new->grp = (struct group *)ft_memalloc(sizeof(struct group))))
	{
		free_files(&new);
		error_ls(d, strerror(errno));
	}
	if ((init_pwd(d, new) == _FAILURE))
		ft_memdel((void *)&new->pwd);
	if ((init_grp(d, new) == _FAILURE))
		ft_memdel((void *)&new->grp);
	return (_SUCCESS);
}
