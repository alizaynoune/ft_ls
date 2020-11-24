/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/17 17:28:05 by alzaynou          #+#    #+#             */
/*   Updated: 2020/11/24 20:43:25 by alzaynou         ###   ########.fr       */
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
	{'u', "--sort_atime", "Use time last access in options (-l) ou (-t)", _U},
	{'S', "--sort_size", "Sort by size", _S_},
	{'f', "--no_sort", "Output is no sorted", _F},
	{'n', "--id_nbr", "Display user and group ID", _N},
	{'s', "--blocks", "Desplay number of blocks used by each file", _S},
	{'T', "--time_info", "When used option '-l' print complet time information",
		_T_},
    {'i', "--inode", "print the index number of each file", _I},
	{'d', "--dir_as_file",
		"Directories are listed as plain files (ignor recursively)", _D},
	{'@', "--xatt", "Display extended attribute key and size in long (-l) output", _XATT},
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


void		parsing_dir(t_all *d)
{
	t_files		*tmp;
    size_t      len;

	tmp = ((d->options & _R)) ?  d->l_arg_file : d->arg_file;
	(d->len[_MAJ_MIN]) ? fix_len_maj_size(d) : 0;
	while (tmp)
	{
		if (!(d->options & _D) && (S_ISDIR(tmp->st->st_mode)))
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
	(d->print_path == _TROUBLE) ? d->print_path = _SUCCESS : 0;
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
