/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/17 17:28:05 by alzaynou          #+#    #+#             */
/*   Updated: 2020/11/06 12:24:01 by alzaynou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

t_op	g_op[_MAX_OP + 1] =
{
	{'l', "--long", "List in long format", _L},
	{'R', "--Recursuvely", "Recursuvely list", _R_},
	{'r', "--reverse", "Reverse list", _R},
	{'a', "--all", "Include dir/files whose name begin with a dot (.)", _A},
	{'t', "--sort_mtime", "Sort by time modifies", _T},
	{'G', "--color", "Enable colorzed output", _G},
	{'u', "--sort_atime", "Sort by time last access", _U},
	{'S', "--sort_size", "Sort by size", _S_},
	{'f', "--no_sort", "Output is no sorted", _F},
	{'n', "--id_nbr", "Display user and group ID", _N},
	{'s', "--blocks", "Desplay number of blocks used by each file", _S},
	{'T', "--time_info", "When used option '-l' print complet time information",
		_T_},
	{0, 0, 0, 0}
};

void		usage_ls(t_all *d, char c, char *str)
{
	if (str)
		ft_dprintf(_OUT, "ft_ls : illegal option %s\n", str);
	else if (c)
		ft_dprintf(_OUT, "ft_ls : illegal option -- %c\n", c);
	ft_dprintf(_OUT, "try ft_ls -h or ft_ls --help\n");
	free_all(d);
	exit(_FAILURE);
}

void		help_ls(t_all *d)
{
	int		i;

	i = -1;
	ft_dprintf(_OUT, "./ft_ls :[options] [file ...]\n");
	while (++i < _MAX_OP)
		ft_printf("\t-%c\t%-13s\t:%s\n", g_op[i].c, g_op[i].str, g_op[i].desc);
	free_all(d);
	exit(_SUCCESS);
}

int			pars_word_option(char *flag, t_all *d)
{
	int		i;

	i = -1;
	if (!(ft_strcmp(flag, "--")))
		return (_SUCCESS);
	else if (!(ft_strcmp(flag, "--help")))
		help_ls(d);
	else
	{
		while (++i < _MAX_OP)
		{
			if (!(ft_strcmp(flag, g_op[i].str)))
			{
				d->options |= g_op[i].valu;
				((d->options & _N)) ? d->options |= _L : 0;
				return (_SUCCESS);
			}
		}
	}
	return (_FAILURE);
}

int			pars_char_option(char c, t_all *d)
{
	int			i;

	i = -1;
	(c == 'h') ? help_ls(d) : 0;
	while (++i < _MAX_OP)
	{
		if (c == g_op[i].c)
		{
			d->options |= g_op[i].valu;
			((d->options & _N)) ? d->options |= _L : 0;
			return (_SUCCESS);
		}
	}
	return (_FAILURE);
}

void		parsing_option(char *flag, t_all *d)
{
	int		i;

	i = 1;
	if (flag[i] == '-')
	{
		((pars_word_option(flag, d)  == _FAILURE)) ? usage_ls(d, 0, flag) : 0;
	}
	else
	{
		while (flag[i])
		(pars_char_option(flag[i], d)) == _SUCCESS ? i++ : usage_ls(d, flag[i], NULL);
	}
}

int			stat_file(t_all *d, char *f, struct stat *st)
{
	if ((stat(f, st)) == -1)
	{
		ft_dprintf(_ERR, "ls : %s: %s\n", f, strerror(errno));
		errno = 0;
		d->ret = _FAILURE;
		return (_FAILURE);
	}
	return (_SUCCESS);
}

int			lstat_file(t_all *d, char *f, struct stat *st)
{
	if ((lstat(f, st)) == -1)
	{
		ft_dprintf(_ERR, "ls : %s: %s\n", f, strerror(errno));
		errno = 0;
		d->ret = _FAILURE;
		return (_FAILURE);
	}
    return (_SUCCESS);
}

int		init_pwd(t_all *d, t_files *new)
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
    if (!pwd)
    {
        d->ret = _FAILURE;
        return (_FAILURE);
    }
	if (!(d->options & _N) && !(new->pwd->pw_name = ft_strdup(pwd->pw_name)))
    {
        free_files(&new);
        error_ls(d, strerror(errno));
    }
	new->pwd->pw_uid = pwd->pw_uid;
    return (_SUCCESS);
}

int			init_grp(t_all *d, t_files *new)
{
    struct group        *grp;

    errno = 0;
    grp = getgrgid(new->st->st_gid);
    if (errno)
    {
        ft_printf("%s\n", new->path);
        ft_dprintf(_ERR, "getgrgid: %s\n", strerror(errno));
		errno = 0;
        free_files(&new);
        d->ret = _FAILURE;
        return (_FAILURE);
    }
    if (!grp)
    {
        d->ret = _FAILURE;
        return (_FAILURE);
    }
	if (!(d->options & _N) && !(new->grp->gr_name = strdup(grp->gr_name)))
    {
        free_files(&new);
        error_ls(d, strerror(errno));
    }
	new->grp->gr_gid = grp->gr_gid;
    return (_SUCCESS);
}

int		init_id(t_all *d, t_files *new)
{
	if (!(new->pwd = (struct passwd *)ft_memalloc(sizeof(struct passwd))) ||
			!(new->grp = (struct group *)ft_memalloc(sizeof(struct group))))
	{
		free_files(&new);
		error_ls(d, strerror(errno));
	}
    if ((init_pwd(d, new) == _FAILURE) || (init_grp(d, new) == _FAILURE))
    {
        free_files(&new);
        return (_FAILURE);
    }
    return (_SUCCESS);
}

t_files		*init_files(t_all *d, char *name, char *path)
{
	t_files		*new;

	if (!(new = (t_files *)ft_memalloc(sizeof(t_files))))
		error_ls(d, strerror(errno));
	if (!(new->st = (struct stat *)ft_memalloc(sizeof(struct stat))))
	{
		free(new);
		error_ls(d, strerror(errno));
	}
	if (!(new->name = ft_strdup(name)) || (!path && !((new->path = ft_strdup(name)))))
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

void		get_len_block(t_all *d, t_files *f)
{
	size_t		len;

	len = ft_intlen(f->st->st_blocks);
	(len > d->len[_BLOCK]) ? d->len[_BLOCK] = len : 0;
}

void		len_major_minor(t_all *d, t_files *f)
{
	size_t		len;

	len = ft_intlen(major(f->st->st_rdev));
	(len > d->len[_MAJ_MIN]) ? d->len[_MAJ_MIN] = len : 0;
	len = ft_intlen(minor(f->st->st_rdev));
	(len > d->len[_MAJ_MIN]) ? d->len[_MAJ_MIN] = len : 0;
	(d->len[_MAJ_MIN] < 3) ? d->len[_MAJ_MIN] = 3 : 0;
}

void		get_lens(t_all *d, t_files *f)
{
	size_t		len;

	len = ft_intlen(f->st->st_nlink);
	((d->len[_LINK] < len)) ? d->len[_LINK] = len : 0;
	len = ft_intlen(f->st->st_size);
	((d->len[_SIZE] < len)) ? d->len[_SIZE] = len : 0;
	len = (!(d->options & _N) && f->pwd && f->pwd->pw_name) ? ft_strlen(f->pwd->pw_name) : 0;
	((d->options & _N) && f->pwd) ? len = ft_intlen(f->pwd->pw_uid) : 0;
	(len > d->len[_OWNER]) ? d->len[_OWNER] = len : 0;
	len = (!(d->options & _N) && f->grp && f->grp->gr_name) ? ft_strlen(f->grp->gr_name) : 0;
	((d->options & _N) && f->grp) ? len = ft_intlen(f->grp->gr_gid) : 0;
	(len > d->len[_GROUP]) ? d->len[_GROUP] = len : 0;
	((d->options & _S)) ? get_len_block(d, f) : 0;
	((S_ISCHR(f->st->st_mode)) || (S_ISBLK(f->st->st_mode))) ? len_major_minor(d, f) : 0;
}

ssize_t			fix_size_link(t_all *d, t_files *f, ssize_t size)
{
	ssize_t		len;

	errno = 0;
	if (!(f->link = (char *)ft_memalloc(sizeof(char) * size + 1)))
	{
		free_files(&f);
		error_ls(d, strerror(errno));
	}
	len = readlink(f->path, f->link, size + 1);
	if (len > size)
		return (_FAILURE);
	else if (len < 0)
		return (len);
	else
		return (_SUCCESS);
}

int			read_link(t_all *d, t_files *f)
{
	ssize_t		size;
	ssize_t		loop;

	if (!(S_ISLNK(f->st->st_mode)))
		return (_SUCCESS);
	if (!(f->link = (char *)ft_memalloc(sizeof(char) * (f->st->st_size + 1))))
	{
		free_files(&f);
		error_ls(d, strerror(errno));
	}
	size = readlink(f->path, f->link, f->st->st_size + 1);
	if (size > f->st->st_size)
	{
		 size = (size) ? size * 2 : 2;
		 loop = _FAILURE;
		while (loop == _FAILURE)
		{
			ft_strdel(&f->link);
			loop = fix_size_link(d, f, size);
			if (loop < 0)
				break ;
			size *= 2;
		}
		(loop < 0) ? size = loop : 0;
	}
	if (size < 0 )
	{
		ft_dprintf(_ERR, "readlink: '%s'  %s\n", strerror(errno), f->path);
		d->ret = _FAILURE;
		errno = 0;
		free_files(&f);
		return (_FAILURE);
	}
	return (_SUCCESS);
}

void		parsing_files(t_all *d, char *f, t_files **lst, t_files **l_lst)
{
	t_files			*file;

	file = init_files(d, f, NULL);
	if ((d->options & _L) && ((lstat_file(d, f, file->st)) == _SUCCESS))
	{
		if (init_id(d, file) == _FAILURE || read_link(d, file) == _FAILURE)
            return ;
		push_files(d, file, lst, l_lst);
		(!(S_ISDIR(file->st->st_mode))) ? get_lens(d, file) : 0;
	}
	else if (!(d->options & _L) && (stat_file(d, f, file->st) == _SUCCESS))
	{
		(d->options & _S) ? get_len_block(d, file) : 0;
		push_files(d, file, lst, l_lst);
	}
	else
		free_files(&file);
}

void		fix_len_maj_size(t_all *d)
{	
	(((d->len[_MAJ_MIN] * 2) + 2) > d->len[_SIZE]) ?
		d->len[_SIZE] = ((d->len[_MAJ_MIN] * 2) + 2) :0;
	(((d->len[_MAJ_MIN] * 2) + 2) < d->len[_SIZE]) ?
		d->len[_MAJ_MIN] = ((d->len[_SIZE] - 2) / 2) : 0;

}

void		parsing_dir(t_all *d)
{
	t_files		*tmp;
    size_t      len;

	tmp = ((d->options & _R)) ?  d->l_arg_file : d->arg_file;
	(d->len[_MAJ_MIN]) ? fix_len_maj_size(d) : 0;
	while (tmp)
	{
		if (!(d->options & _L) && (S_ISLNK(tmp->st->st_mode)))
			(S_ISDIR(tmp->st->st_mode)) ? push_waiting(d, &*tmp)
				: print_files(d, tmp);
		else if ((S_ISDIR(tmp->st->st_mode)))
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
}

void		parsing_arg(int ac, char **av, t_all *d)
{
	int		i;

	i = -1;
	while (++i < ac)
		(av[i][0] == '-') ? parsing_option(av[i], d) : 0;
	i = -1;
	while (++i < ac)
	((av[i][0] != '-') || (av[i][0] == '-' && !av[i][1])) ?  parsing_files(d, av[i], &d->arg_file, &d->l_arg_file) : 0;
}

void        start_curr(t_all *d)
{
	if (d->ret == _FAILURE)
		return ;
    parsing_files(d, ".", &d->arg_file, &d->l_arg_file);
    parsing_dir(d);
}

int			main(int ac, char **av)
{
	int		ret;
	t_all	*d;

	errno = 0;
	if (!(d = (t_all *)ft_memalloc(sizeof(t_all))))
		error_ls(NULL, strerror(errno));
	d->ret = _SUCCESS;
	d->print_path = _FAILURE;
	if (ac > 1)
		parsing_arg(ac - 1, &av[1], d);
	(d->arg_file) ? parsing_dir(d) : start_curr(d);
    (d->head_waiting) ? loop_dir(d) : 0;
    ret = d->ret;
	free_all(d);
	return (ret);
}
