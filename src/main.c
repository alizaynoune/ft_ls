/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/17 17:28:05 by alzaynou          #+#    #+#             */
/*   Updated: 2020/10/25 20:16:23 by alzaynou         ###   ########.fr       */
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
		perror(f);
		d->ret = _FAILURE;
		return (_FAILURE);
	}
	return (_SUCCESS);
}

int			lstat_file(t_all *d, char *f, struct stat *l_st)
{
	if ((lstat(f, l_st)) == -1)
	{
		perror(f);
		d->ret = _FAILURE;
		return (_FAILURE);
	}
	return (_SUCCESS);
}

t_files		*init_files(t_all *d, char *name)
{
	t_files		*new;

	if (!(new = (t_files *)ft_memalloc(sizeof(t_files))))
		error_ls(d, strerror(errno));
	if (!(new->l_st = (struct stat *)ft_memalloc(sizeof(struct stat))))
	{
		free(new);
		error_ls(d, strerror(errno));
	}
	if (!(new->name = ft_strdup(name)))
	{
		free_files(new);
		error_ls(d, strerror(errno));
	}
	return (new);
}

void		parsing_files(t_all *d, char *f, t_files **lst, t_files **l_lst)
{
	t_files			*file;

	file = init_files(d, f);
	if ((lstat_file(d, f, file->l_st)) == _SUCCESS)
	{
		if (((file->l_st->st_mode & S_IFMT) == S_IFLNK))
		{
			if (!(file->st = (struct stat *) ft_memalloc(sizeof(struct stat))))
			{
				free_files(file);
				error_ls(d, strerror(errno));
			}
			if ((stat_file(d, f, file->st) == _FAILURE))
				ft_voidfree((void *)&file->st);
		}
		push_files(d, file, lst, l_lst);
	}
	else
		free_files(file);
}

void		parsing_dir(t_all *d)
{
	t_files		*tmp;
    size_t      len;

	tmp = ((d->options & _R)) ?  d->l_arg_file : d->arg_file;
	while (tmp)
	{
		if (!(d->options & _L) && ((tmp->l_st->st_mode & S_IFMT) == S_IFLNK))
		{
			if (((tmp->st->st_mode & S_IFMT) == S_IFDIR))
				push_waiting(d, &*tmp);
			else
				print_files(d, tmp);
		}
		else if (((tmp->l_st->st_mode & S_IFMT) == S_IFDIR))
        {
            len = ft_strlen(tmp->name);
            (tmp->name[len - 1] == '/') ? tmp->name[len - 1] = 0 : 0;
            push_waiting(d, tmp);
        }
		else
			print_files(d, tmp);// remove et;
		tmp = ((d->options & _R)) ? tmp->prev : tmp->next;
	}
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
    parsing_files(d, ".", &d->arg_file, &d->l_arg_file);
    parsing_dir(d);
}

int			main(int ac, char **av)
{
	int		ret;
	t_all	*d;

	ret = _SUCCESS;
	if (!(d = (t_all *)ft_memalloc(sizeof(t_all))))
		error_ls(NULL, strerror(errno));
	if (ac > 1)
		parsing_arg(ac - 1, &av[1], d);
	(d->arg_file) ? parsing_dir(d) : start_curr(d);
    (d->head_waiting) ? loop_dir(d) : 0;
    ret = d->ret;
	free_all(d);
	return (ret);
}
