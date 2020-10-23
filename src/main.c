/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/17 17:28:05 by alzaynou          #+#    #+#             */
/*   Updated: 2020/10/23 19:44:14 by alzaynou         ###   ########.fr       */
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

void		parsing_files(t_all *d, char *f)
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
		push_files(d, file, &d->arg_file, &d->l_arg_file);
	}
	else
		free_files(file);
}

void		parsing_arg(int ac, char **av, t_all *d)
{
	int		i;

	i = -1;
	while (++i < ac)
		(av[i][0] == '-') ? parsing_option(av[i], d) : 0;
	i = -1;
	while (++i < ac)
	(av[i][0] != '-') ?  parsing_files(d, av[i]) : 0;
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
	ret = d->ret;
	free_all(d);
	return (ret);
}
