/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_options.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/24 19:28:34 by alzaynou          #+#    #+#             */
/*   Updated: 2020/11/24 20:43:59 by alzaynou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int         pars_word_option(char *flag, t_all *d)
{
	int     i;

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

int         pars_char_option(char c, t_all *d)
{
	int         i;

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

void        override_active_options(t_all *d)
{
	(d->options & _N) ? d->options |= _L : 0;
	(d->options & _D) ? d->options -= (d->options & _R_) : 0;
	(d->options & _F) ? d->options -= (d->options & _R) : 0;
	(d->options & _F) ? d->options |= _A : 0;
}

void        parsing_option(char *flag, t_all *d)
{
	int     i;

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
