/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_arg.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 10:42:51 by alzaynou          #+#    #+#             */
/*   Updated: 2020/11/27 10:58:08 by alzaynou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		parsing_arg(int ac, char **av, t_all *d)
{
	int		i;

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

int			pars_word_option(char *flag, t_all *d)
{
	int		i;

	i = -1;
	if (!flag[2] && !(ft_strcmp(flag, "--")))
		return (_SUCCESS);
	else if (flag[2] == 'h' && !(ft_strcmp(flag, "--help")))
		help_ls(d);
	else
	{
		while (++i < _MAX_OP)
		{
			if (!(ft_strcmp(flag, g_op[i].str)))
			{
				d->options |= g_op[i].value;
				return (_SUCCESS);
			}
		}
	}
	return (_FAILURE);
}

int			pars_char_option(char c, t_all *d)
{
	int		i;

	i = -1;
	(c == 'h') ? help_ls(d) : 0;
	while (++i < _MAX_OP)
	{
		if (c == g_op[i].c)
		{
			d->options |= g_op[i].value;
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
		((pars_word_option(flag, d) == _FAILURE)) ? usage_ls(d, 0, flag) : 0;
	}
	else
	{
		while (flag[i])
			(pars_char_option(flag[i], d)) == _SUCCESS ? i++ :
				usage_ls(d, flag[i], NULL);
	}
}
