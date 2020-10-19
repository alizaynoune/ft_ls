/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/17 17:28:05 by alzaynou          #+#    #+#             */
/*   Updated: 2020/10/18 14:31:33 by alzaynou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		usage_ls(t_all *d, char c, char *str)
{
	if (str)
		ft_dprintf(0, "ls : illegal option -- %s\n", str);
	else if (c)
		ft_dprintf(0, "ls : illegal option -- %c\n", c);
	ft_dprintf(0, "usage: ls [-larRt --no-sort] [file ...]\n");
	free(d); // dont forget free all data (^_^);
	exit(_FAILURE);
}

void		help_ls(t_all *d)
{
	ft_dprintf(0, "ls :[options] [file ...]\n");
	ft_dprintf(0, "\toptions => -larRt --no-sort\n");
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
	if (c == 'h')
		help_ls(d);
	return (_SUCCESS);
}

void		parsing_option(char *flag, t_all *d)
{
	int		i;

	i = 1;
	if (flag[i] == '-' && (pars_word_option(flag, d) == _SUCCESS))
		return ;
	while (flag[i])
		(pars_char_option(flag[i], d)) == _SUCCESS ? i++ : error_ls(d, flag);
}

void		parsing_arg(int ac, char **av, t_all *d)
{
	int		i;

	i = -1;
	while (++i < ac)
		(av[i][0] == '-') ? parsing_option(av[i], d) : 0;
}

int			main(int ac, char **av)
{
	int		ret;
	t_all	*d;

	ret = _SUCCESS;
	if (!(d = ft_memalloc(sizeof(t_all))))
		error_ls(NULL, strerror(errno));
	if (ac > 1)
		parsing_arg(ac - 1, &av[1], d);
	free(d);//
	return (ret);
}
