#include "ft_ls.h"
#include <stdio.h>

void	free_content(t_content *content)
{
	t_content		*ptr;

	while (content)
	{
		ptr = content;
		content = content->next;
		ptr->n_file ? ft_strdel(&ptr->n_file) : 0;
		free(ptr);
	}
}

void	free_dir(t_dir *dir)
{
	t_dir	*ptr;

	while(dir)
	{
		ptr = dir;
		dir = dir->next;
		ptr->content ? free_content(ptr->content) : 0;
		ptr->n_dir ? ft_strdel(&ptr->n_dir) : 0;
		closedir(ptr->p_dir);
		free(ptr);
	}
}

void	_success(t_struct *d)
{
	if (d)
	{
		d->dir ? free_dir(d->dir) : 0;
		free(d);
	}
}

void	_error(t_struct *d, char *str, int error)
{
	if (d)
	{
		d->dir ? free_dir(d->dir) : 0;
		free(d);
	}
	ft_dprintf(1, "%s\n", str);
	ft_strdel(&str);
	exit(_TROUBLE);
}


void	_invalid(t_struct *d, char c)
{
	if (d)
	{
		d->dir ? free_dir(d->dir) : 0;
		free(d);
	}
	ft_dprintf(1, "ft_ls: invalid option -- '%c'\n", c);
	exit(_TROUBLE);
}

void	parse_options(char *options, t_struct *d)
{
	int	i;

	i = 1;
	while (options[i])
	{
		if (options[i] == 'l')
			d->options |= _L;
		else if (options[i] == 'R')
			d->options |= R_R;
		else if (options[i] == 'r')
			d->options |= _R;
		else if (options[i] == 'a')
			d->options |= _A;
		else if (options[i] == 't')
			d->options |= _T;
		else
			_invalid(d, options[i]);
		i++;
	}
}

void	parse_arg(int ac, char **av, t_struct *d)
{
	size_t		i;
	int		curr_dir;

	i = 0;
	curr_dir = 1;
	while (++i < ac)
		av[i][0] == '-' && av[i][1] ? parse_options(av[i], d) : 0;
	i = 0;
	while (++i < ac)
	{
		if (!(av[i][0] == '-' && av[i][1]))
		{
			read_dir(d, ft_strdup(av[i]));
			curr_dir = 0;
		}
		i++;
	}
	curr_dir ? read_dir(d, ft_strdup(".")) : 0;
}

int	main(int ac, char **av)
{
	t_struct	*data;

	data = NULL;
	if (!(data = (t_struct *)ft_memalloc(sizeof(t_struct))))
		_error(data, ft_strdup(strerror(errno)), _TROUBLE);
	ac > 1 ? parse_arg(ac, av, data) : read_dir(data, ft_strdup("."));
	print_ls(data);
	_success(data);
	return (_SUCCESS);
}
