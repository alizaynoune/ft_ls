/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/17 17:28:05 by alzaynou          #+#    #+#             */
/*   Updated: 2020/10/21 16:34:58 by alzaynou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		usage_ls(t_all *d, char c, char *str)
{
	if (str)
		ft_dprintf(0, "ft_ls : illegal option %s\n", str);
	else if (c)
		ft_dprintf(0, "ft_ls : illegal option -- %c\n", c);
	ft_dprintf(0, "usage: ./ft_ls [-%s --no-sort] [file ...]\n", _OPTIONS);
	free(d); // dont forget free all data (^_^);
	exit(_FAILURE);
}

void		help_ls(t_all *d)
{
	ft_dprintf(0, "./ft_ls :[options] [file ...]\n");
	ft_dprintf(0, "\toptions => -%s --no-sort\n", _OPTIONS);
	ft_dprintf(0, "\t-l : list in long format\n");
	ft_dprintf(0, "\t-a : include directory or file enties whoes %s",
			"names begin whit a dot (.)\n");
	ft_dprintf(0, "\t-r : reverse the order listing\n");
	ft_dprintf(0, "\t-R : recursively list subdirectories encountered\n");
	ft_dprintf(0, "\t-t : sort by time modified\n");
	ft_dprintf(0, "\t-G : colorize the output\n");
	ft_dprintf(0, "\t--no-sort : do not sort\n");
	free(d); ///dont forget free all data (^_^)
	exit(_SUCCESS);
}

int			pars_word_option(char *flag, t_all *d)
{
	if (!(ft_strcmp(flag, "--")))
		return (_SUCCESS);
	if (!(ft_strcmp(flag, "--no-sort")))
		d->options |= _NO_SORT;
	else if (!(ft_strcmp(flag, "--help")))
		help_ls(d);
	else
		return (_FAILURE);
	return (_SUCCESS);
}

int			pars_char_option(char c, t_all *d)
{
	if (!(ft_strchr(_OPTIONS, c)))
		return (_FAILURE);
	(c == 'h') ? help_ls(d) : 0;
	(c == 'l') ? d->options |= _L : 0;
	(c == 'a') ? d->options |= _A : 0;
	(c == 'r') ? d->options |= _R : 0;
	(c == 'R') ? d->options |= _R_ : 0;
	(c == 't') ? d->options |= _T : 0;
	(c == 'G') ? d->options |= _G : 0;
	(c == 'U') ? d->options |= _U : 0;
	(c == 'S') ? d->options |= _S : 0;
	return (_SUCCESS);
}

void		parsing_option(char *flag, t_all *d)
{
	int		i;
	int		loop;

	i = 1;
	loop = _SUCCESS;
	if (flag[i] == '-')
	{
		loop = pars_word_option(flag, d);
		(loop == _FAILURE) ? usage_ls(d, 0, flag) : 0;
	}
	else
	{
		while (flag[i])
		(pars_char_option(flag[i], d)) == _SUCCESS ? i++ : usage_ls(d, flag[i], NULL);
	}
}

int			stat_file(char *f, struct stat *st)
{
	if ((lstat(f, st)) == -1)
	{
		perror(f);
		return (_FAILURE);
	}
	return (_SUCCESS);
}

int			lstat_file(char *f, struct stat *l_st)
{
	if ((lstat(f, l_st)) == -1)
	{
		perror(f);
		return (_FAILURE);
	}
	return (_SUCCESS);
}

void		parsing_files(t_all *d, char *f)
{
	struct stat		*st;
	struct stat		*l_st;
	DIR				*test;

	if (!(st = (struct stat *)ft_memalloc(sizeof(struct stat))))
		error_ls(d, strerror(errno));
	if ((stat_file(f, st)) == _SUCCESS)
	{
		if (((st->st_mode & S_IFMT) == S_IFDIR))
			ft_dprintf(0, "d==>");// push to dir list
		else if (((st->st_mode & S_IFMT) == S_IFLNK))
		{
			if (!(l_st = (struct stat *) ft_memalloc(sizeof(struct stat))))
				error_ls(d, strerror(errno));
			if ((lstat_file(f, l_st) == _FAILURE))
				free(l_st);
			else
			{
				if ((test = opendir(f)))
					ft_dprintf(0, "open  ");
				ft_dprintf(0, "l==> "); // if dir push to dir list else push to file list
			}
			// dont forget the name of file (^_^)
		}
		ft_dprintf(0, "%s\n", f);
	}
	else
		free(st);
}

void		parsing_arg(int ac, char **av, t_all *d)
{
	int		i;

	i = -1;
	while (++i < ac)
		(av[i][0] == '-') ? parsing_option(av[i], d) : parsing_files(d, av[i]);
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
	free(d);//
	return (ret);
}
