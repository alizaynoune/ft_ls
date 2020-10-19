#ifndef FT_LS_H
# define FT_LS_H

# include "ft_dprintf.h"
# include <sys/stat.h>
# include <dirent.h>
# include <stdio.h>
# include <errno.h>
# include <time.h>
# include <pwd.h>
# include <sys/ioctl.h>
# include <grp.h>

# define _SUCCESS	(0)
# define _FAILURE	(1)
# define _TROUBLE	(2)

# define _L		(1)
# define R_R		(2)
# define _R		(4)
# define _A		(8)
# define _T		(16)
# define _U		(32)
# define _G		(64)

# define _X_		(1)
# define W_		(2)
# define R_		(4)

# define LINK_		(0)
# define OWNER_		(1)
# define GROUP_		(2)
# define SIZE_		(3)
# define NAME_		(4)


# define C_BLK		("")
# define C_CHR		("\e[1;33m")
# define C_DIR		("\e[1;34m")
# define C_FIFO		("\e[1;90m")
# define C_LNK		("\e[1;96m")
# define C_SOCK		("\e[1;35m")
# define C_EXE		("\e[1;32m")
# define C_DEF		("\e[1;0m")
# define C_ERROR	("\e[1;31m")

typedef struct			s_content
{
	char			*path;
	char			*name;
	struct stat		*st;
	char			*owner;
	char			*group;
	struct s_content	*prev;
	struct s_content	*next;
}				t_content;

typedef struct		s_dir
{
	t_content	*content;
	char		*name;
	unsigned long long total;
	struct s_dir	*next;
	struct s_dir	*prev;
	size_t		len[5];
	t_content	*lst_content;
}			t_dir;

typedef struct		s_start
{
	char		*name;
	struct stat	*st;
	struct s_start	*next;
	struct s_start	*prev;
}			t_start;

typedef struct		s_all
{
	int		options;
	int		ret;
	t_dir		*dir;
	t_dir		*files;
	t_dir		*lst_files;
	t_start		*start_dir;
	t_dir		*lst_dir;
	t_dir		*curr;
}			t_all;





void		_loop_dir(t_all *all);
void		_failed(t_all *all, char *err);
void		_free_content(t_content *content);
void		_print_out(t_all *all);
int		_ls_cmp(const char *s1, const char *s2);

# endif
