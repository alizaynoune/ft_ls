/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/25 19:08:22 by alzaynou          #+#    #+#             */
/*   Updated: 2020/10/25 19:20:57 by alzaynou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		print_files(t_all *d, t_files *f)
{
	if ((d->options & _L))
	{
		//print mode
		// print nlink
		// print owner
		// print group
		// print size
		// print time
		ft_printf("%s  lllll\n", f->name);
        // if is link print name of stat file
	}
	else
		ft_printf("%s\n", f->name);
}
