/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 10:43:29 by alzaynou          #+#    #+#             */
/*   Updated: 2020/11/27 11:46:53 by alzaynou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

t_files			*cmp_ascii(t_files *f, t_files *lst)
{
	t_files		*tmp;

	tmp = lst;
	while (tmp)
	{
		if (ft_strcmp(f->name, tmp->name) < 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

t_files			*cmp_time(t_files *f, t_files *lst, int options)
{
	t_files		*tmp;
	time_t		f_time;
	time_t		t_time;

	tmp = lst;
	f_time = (options & _U) ? f->st->st_atime : f->st->st_mtime;
	while (tmp)
	{
		t_time = (options & _U) ? tmp->st->st_atime : tmp->st->st_mtime;
		if (f_time > t_time)
			return (tmp);
		else if (f_time == t_time)
		{
			if (ft_strcmp(f->name, tmp->name) < 0)
				return (tmp);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

t_files			*cmp_size(t_files *f, t_files *lst)
{
	t_files		*tmp;

	tmp = lst;
	while (tmp)
	{
		if (f->st->st_size > tmp->st->st_size)
			return (tmp);
		else if (f->st->st_size == tmp->st->st_size)
		{
			if (ft_strcmp(f->name, tmp->name) < 0)
				return (tmp);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

t_files			*get_position(t_all *d, t_files *f, t_files *lst)
{
	if ((d->options & _S_))
		return (cmp_size(f, lst));
	else if ((d->options & _T))
		return (cmp_time(f, lst, d->options));
	else
		return (cmp_ascii(f, lst));
}

void			sort_arg(char **av, int ac)
{
	int			i;
	int			j;
	char		*bck;

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
