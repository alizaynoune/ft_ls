/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_files.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/24 19:30:53 by alzaynou          #+#    #+#             */
/*   Updated: 2020/11/24 20:43:52 by alzaynou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void        parsing_files(t_all *d, char *f, t_files **lst, t_files **l_lst)
{
	t_files         *file;

	file = init_files(d, f, NULL);
	if ((lstat_file(f, file->st) == _SUCCESS))
	{
		(d->options & _L) ? init_id(d, file) : 0;
		(S_ISLNK(file->st->st_mode)) ? parsing_link_arg(d, file) : 0;
		push_files(d, file, lst, l_lst);
		(!(S_ISDIR(file->st->st_mode))) ? get_lens(d, file) : 0;
	}
	else
	{
		ft_dprintf(_ERR, "ls : %s: %s\n", f, strerror(errno));
		errno = 0;
		d->ret = _FAILURE;
		free_files(&file);
		d->print_path = _TROUBLE;
	}
}

void        fix_len_maj_size(t_all *d)
{
	if (((d->len[_MAJ_MIN] * 2) + 2) > d->len[_SIZE])
		d->len[_SIZE] = ((d->len[_MAJ_MIN] * 2) + 2);
	else
	{
		(d->len[_SIZE] % 2) ? d->len[_SIZE]++ : 0;
		d->len[_MAJ_MIN] = ((d->len[_SIZE] - 2) / 2);
	}

}

void        parsing_link_arg(t_all *d, t_files *f)
{
	struct stat     *st;

	read_link(d, f);
	if (d->options & _L)
		return ;
	if (!(st = (struct stat *)ft_memalloc(sizeof(struct stat))))
	{
		free_files(&f);
		error_ls(d, strerror(errno));
	}
	if (lstat_file(f->link, st) == _SUCCESS)
	{
		ft_memdel((void *)&f->st);
		f->st = st;
	}
	else
	{
		ft_memdel((void *)&st);
	}
}
int         stat_file(char *f, struct stat *st)
{
	if ((stat(f, st)) == -1)
		return (_FAILURE);
	return (_SUCCESS);
}

int         lstat_file(char *f, struct stat *st)
{
	if ((lstat(f, st)) == -1)
		return (_FAILURE);
	return (_SUCCESS);
}
