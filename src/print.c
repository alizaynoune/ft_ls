#include "ft_ls.h"

char	*_color(unsigned char type)
{
	if (type == DT_REG)
		return (DEF);
	if (type == DT_DIR)
		return (PURPLE);
	if (type == DT_LNK)
		return (BLUE);
	return (DEF);
}

void		print_content(t_content *content, unsigned char options)
{
	t_content	*ptr;

	while (content)
	{
		ptr = content;
		content = content->next;
		ft_dprintf(1, "%s%s  ", _color(ptr->dirent->d_type), ptr->dirent->d_name);
		(options & _L) ? ft_dprintf(1, "\n") : 0;
	}
	(!(options & _L)) ? ft_dprintf(1, "%s\n", DEF) : 0;
}

void		print_ls(t_struct *d)
{
	t_dir	*ptr;

	ptr = d->dir;
	while (ptr)
	{
		ft_dprintf(1, "%s:\n", ptr->n_dir);
		print_content(ptr->content, d->options);
		(ptr = ptr->next) ? ft_dprintf(1, "\n") : 0;
	}
}
