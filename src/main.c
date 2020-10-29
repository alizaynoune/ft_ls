/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/17 17:28:05 by alzaynou          #+#    #+#             */
/*   Updated: 2020/10/29 03:27:36 by alzaynou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

t_op	g_op[_MAX_] =
{
	{'l', "--long", "list in long format", _L},
	{'R', "--Recursuvely", "Recursuvely list", _R_},
	{'r', "--reverse", "reverse list", _R},
	{'a', "--all", "include dir/files whose name begin with a dot (.)", _A},
	{'t', "--sort_mtime", "sort by time modifies", _T},
	{'G', "--color", "enable colorzed output", _G},
	{'u', "--sort_atime", "sort by time last access", _U},
	{'S', "--sort_size", "sort by size", _S},
	{'f', "--no_sort", "output is no sorted", _F},
	{0, 0, 0, 0}
};

void		usage_ls(t_all *d, char c, char *str)
{
	if (str)
		ft_dprintf(0, "ft_ls : illegal option %s\n", str);
	else if (c)
		ft_dprintf(0, "ft_ls : illegal option -- %c\n", c);
	ft_dprintf(0, "try ft_ls -h or ft_ls --help\n");
	free_all(d);
	exit(_FAILURE);
}

void		help_ls(t_all *d)
{
	int		i;

	i = -1;
	ft_dprintf(0, "./ft_ls :[options] [file ...]\n");
	while (++i < _MAX_ - 1)
		ft_printf("\t%c\t%-13s\t:%s\n", g_op[i].c, g_op[i].str, g_op[i].desc);
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
		while (++i < _MAX_ - 1)
		{
			if (!(ft_strcmp(flag, g_op[i].str)))
			{
				d->options |= g_op[i].valu;
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
	while (++i < _MAX_ - 1)
	{
		if (c == g_op[i].c)
		{
			d->options |= g_op[i].valu;
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
		ft_dprintf(2, "ls : %s: %s\n", f, strerror(errno));
		d->ret = _FAILURE;
		return (_FAILURE);
	}
	return (_SUCCESS);
}

int			lstat_file(t_all *d, char *f, struct stat *st)
{
	if ((lstat(f, st)) == -1)
	{
		ft_dprintf(2, "ls : %s: %s\n", f, strerror(errno));
		d->ret = _FAILURE;
		return (_FAILURE);
	}
	return (_SUCCESS);
}

int		init_pwd(t_all *d, t_files *new, struct passwd *pwd)
{
	if (!(new->pwd->pw_name = ft_strdup(pwd->pw_name)))
		return (_FAILURE);
	new->pwd->pw_uid = pwd->pw_uid;
	return (_SUCCESS);
}

int			init_grp(t_all *d, t_files *new, struct group *grp)
{
	if (!(new->grp->gr_name = strdup(grp->gr_name)))
		return (_FAILURE);
	new->grp->gr_gid = grp->gr_gid;
	return (_SUCCESS);
}

void		init_id(t_all *d, t_files *new)
{
	struct passwd		*pwd;
	struct group		*grp;

	if (!(new->pwd = (struct passwd *)ft_memalloc(sizeof(struct passwd))) ||
			!(new->grp = (struct group *)ft_memalloc(sizeof(struct group))))
	{
		free_files(&new);
		error_ls(d, strerror(errno));
	}
	errno = 0;
	pwd = getpwuid(new->st->st_uid);
	(errno) ? ft_dprintf(2, "getpwuid: %s\n", strerror(errno)) : 0;
	if (pwd && init_pwd(d, new, pwd) == _FAILURE)
	{
		free_files(&new);
		error_ls(d, strerror(errno));
	}
	errno = 0;
	grp = getgrgid(new->st->st_gid);
	if (grp && init_grp(d, new, grp) == _FAILURE)
	{
		free_files(&new);
		error_ls(d, strerror(errno));
	}
	(errno) ? ft_dprintf(2, "getgrgid: %s\n", strerror(errno)) : 0;
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
	if (!(new->name = ft_strdup(name)) || !(new->path = ft_strdup(path)))
	{
		free_files(&new);
		error_ls(d, strerror(errno));
	}
	return (new);
}

void		get_lens(t_all *d, t_files *f, char *name)
{
	size_t		len;

	//ft_memset((void *)d->len, 0, _MAX_LEN_TABLE);
	len = ft_intlen(f->st->st_nlink);
    //ft_printf("[%d %s]", len, name);
	((d->len[_LINK] < len)) ? d->len[_LINK] = len : 0;
	len = ft_intlen(f->st->st_size);
	((d->len[_SIZE] < len)) ? d->len[_SIZE] = len : 0;
	len = (f->pwd && f->pwd->pw_name) ? ft_strlen(f->pwd->pw_name) : 0;
	(len > d->len[_OWNER]) ? d->len[_OWNER] = len : 0;
	len = (f->grp && f->grp->gr_name) ? ft_strlen(f->grp->gr_name) : 0;
	(len > d->len[_GROUP]) ? d->len[_GROUP] = len : 0;

}

void		parsing_files(t_all *d, char *f, t_files **lst, t_files **l_lst)
{
	t_files			*file;

	file = init_files(d, f, f);
	if ((d->options & _L) && ((lstat_file(d, f, file->st)) == _SUCCESS))
	{
		init_id(d, file);
		push_files(d, file, lst, l_lst);
		((file->st->st_mode & S_IFMT) != S_IFDIR) ? get_lens(d, file, f) : 0;
	}
	else if (!(d->options & _L) && (stat_file(d, f, file->st) == _SUCCESS))
		push_files(d, file, lst, l_lst);
	else
		free_files(&file);
}

void		parsing_dir(t_all *d)
{
	t_files		*tmp;
    size_t      len;

	tmp = ((d->options & _R)) ?  d->l_arg_file : d->arg_file;
	while (tmp)
	{
		if (!(d->options & _L) && ((tmp->st->st_mode & S_IFMT) == S_IFLNK))
			(((tmp->st->st_mode & S_IFMT) == S_IFDIR)) ?
				push_waiting(d, &*tmp) : print_files(d, tmp);
		else if (((tmp->st->st_mode & S_IFMT) == S_IFDIR))
        {
            len = ft_strlen(tmp->name);
            (tmp->name[len - 1] == '/') && len > 1 ? tmp->name[len - 1] = 0 : 0;
            push_waiting(d, tmp);
        }
		else
		{
			d->print_path = _SUCCESS;
			print_files(d, tmp);// remove et;
		}
		tmp = ((d->options & _R)) ? tmp->prev : tmp->next;
	}
	free_files(&d->arg_file);
}

void		parsing_arg(int ac, char **av, t_all *d)
{
	int		i;

	i = -1;
	while (++i < ac)
		(av[i][0] == '-') ? parsing_option(av[i], d) : 0;
	i = -1;
	while (++i < ac)
	(av[i][0] != '-') ?  parsing_files(d, av[i], &d->arg_file, &d->l_arg_file) : 0;
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
