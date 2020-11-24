/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_arg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/24 19:01:10 by alzaynou          #+#    #+#             */
/*   Updated: 2020/11/24 20:43:41 by alzaynou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void        sort_arg(char **av, int ac)
{
	int     i;
	int     j;
	char    *bck;

	i = 0;
	while (i < ac)
	{
		j = i + 1;
		while (j < ac)
		{
			if (ft_strcmp(av[i], av[j]) > 0)
			{
				bck = av[i];
				av[i] = av[j];
				av[j] = bck;
			}
			j++;
		}
		i++;
	}
}

void        parsing_arg(int ac, char **av, t_all *d)
{
	int     i;

	i = -1;
	while (++i < ac)
		(av[i][0] == '-') ? parsing_option(av[i], d) : 0;
	override_active_options(d);
	(d->options & _F) ? 0 : sort_arg(av, ac);
	i = -1;
	while (++i < ac)
		((av[i][0] != '-') || (av[i][0] == '-' && !av[i][1])) ?
			parsing_files(d, av[i], &d->arg_file, &d->l_arg_file) : 0;
}
