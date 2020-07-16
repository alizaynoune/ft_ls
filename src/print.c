#include "ft_ls.h"

char	*_color(unsigned char type)
{
	if (type == DT_REG)
		return (DEF);
	if (type == DT_DIR)
		return (PURPLE);
	if (type == DT_LNK)
		return (BLUE);
	if (type == DT_CHR)
		return (YELLOW);
	return (DEF);
}

void		_mode(t_struct *d, int mode)
{
	char	*ptr;
	int	i;

	i = 0;
	if (!(ptr = ft_itoa_base(mode, 8)))
		_error(d, ft_strdup(strerror(errno)), _TROUBLE);
	while (ptr[i] && i < 3)
	{
		((ptr[i] - '0') & R_) ? ft_dprintf(1, "r") : ft_dprintf(1, "-");
		((ptr[i] - '0') & W_) ? ft_dprintf(1, "w") : ft_dprintf(1, "-");
		((ptr[i] - '0') & X_) ? ft_dprintf(1, "x") : ft_dprintf(1, "-");
		i++;
	}
	ft_dprintf(1, " ");
	free(ptr);
}

void		_type(t_struct *d, unsigned int type)
{
	(type == DT_REG) ? ft_dprintf(1, "-") : 0 ;
	(type == DT_BLK) ? ft_dprintf(1, "b") : 0 ;
	(type == DT_CHR) ? ft_dprintf(1, "c") : 0 ;
	(type == DT_DIR) ? ft_dprintf(1, "d") : 0 ;
	(type == DT_LNK) ? ft_dprintf(1, "l") : 0 ;
	(type == DT_FIFO) ? ft_dprintf(1, "p") : 0 ;
	(type == DT_SOCK) ? ft_dprintf(1, "s") : 0 ;
	(type == DT_UNKNOWN) ? ft_dprintf(1, "?") : 0;
}

void		_list_print(t_struct *d, t_content *ptr)
{
	_type(d, ptr->dirent->d_type);
	_mode(d, ptr->st.st_mode & (~S_IFMT));
	ft_dprintf(1, "%ld ", ptr->st.st_nlink);
	ft_dprintf(1, "%-4s %-4s  %7lld   %s ", ptr->owner, ptr->group, ptr->st.st_size, ptr->time);
}

void		print_content(t_struct *d, t_content *content, unsigned char options)
{
	t_content	*ptr;
	struct stat	st;
	unsigned long long total;

	total = 0;
	while (content)
	{
		ptr = content;
		content = content->next;
		(d->options & _L) ? _list_print(d, ptr) : 0;
		ft_dprintf(1, "%s%s%s", _color(ptr->dirent->d_type), ptr->dirent->d_name, DEF);
//		ft_dprintf(1, " ---%llu---", total);
		(d->options & _L) ? ft_dprintf(1, "\n") : ft_dprintf(1, " ");
		total += ptr->st.st_blocks;
	}
	(!(d->options & _L)) ? ft_dprintf(1, "\n", total) : 0;
}

void		print_ls(t_struct *d)
{
	t_dir	*ptr;

	ptr = d->dir;
	while (ptr)
	{
		(d->options & R_R) ? ft_dprintf(1, "%s:\n", ptr->n_dir) : 0;
		print_content(d, ptr->content, d->options);
		(ptr = ptr->next) ? ft_dprintf(1, "\n") : 0;
	}
}
