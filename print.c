# include "ft_ls.h"

void		_print_sample(t_all *all)
{
	t_dir		*dir;
	t_content	*content;

	dir = all->dir;
	while (dir)
	{
		content = dir->content;
		while (content)
		{
			ft_dprintf(1, "%s    ", content->dirent->d_name);
			content = content->next;
		}
		ft_dprintf(1, "\n");
		dir = dir->next;
	}
}

char		_print_type(unsigned char type)
{
	type == DT_BLK ? ft_dprintf(1, "b") : 0;
	type == DT_CHR ? ft_dprintf(1, "c") : 0;
	type == DT_DIR ? ft_dprintf(1, "d") : 0;
	type == DT_FIFO ? ft_dprintf(1, "p") : 0;
	type == DT_LNK ? ft_dprintf(1, "l") : 0;
	type == DT_REG ? ft_dprintf(1, "-") : 0;
	type == DT_SOCK ? ft_dprintf(1, "s") : 0;
	type == DT_UNKNOWN ? ft_dprintf(1, "?") : 0;
}

void		_print_list(t_all *all)
{
	t_dir		*dir;
	t_content	*content;

	dir = all->dir;
	while (dir)
	{
		content = dir->content;
		ft_dprintf(1, "%s/:\n", dir->name);
		while (content)
		{
			_print_type(content->dirent->d_type);
		//	_mode(content->st->st_mode);
			ft_dprintf(1, "   %s\n", content->dirent->d_name);
			content = content->next;
		}
		dir = dir->next;
	}
}
