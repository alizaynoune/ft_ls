#ifndef FT_LS_H
# define FT_LS_H

# include "ft_dprintf.h"
# include <sys/stat.h>
# include <dirent.h>
# include <stdio.h>
# include <errno.h>
# include <time.h>
# include <pwd.h>
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

# define X_		(1)
# define W_		(2)
# define R_		(4)

# define DEF		("\e[0m")
# define RED		("\e[1;31m")
# define GREEN		("\e[1;32m")
# define YELLOW		("\e[1;33m")
# define BLUE		("\e[1;96m")
# define MAGENTA	("\e[1;95m")
# define PURPLE		("\e[1;34m")
# define SILVER		("\e[1;90m")


typedef struct			s_content
{
	char			*n_file;
	char			*parent;
	char			*time;
	struct stat		st;
	//struct passwd		*passwd;
	char			*owner;
	char			*group;
	struct dirent		*dirent;
	struct s_content	*next;
	struct s_content	*prev;
}				t_content;

typedef struct			s_dir
{
	char			*n_dir;
	DIR			*p_dir;
	struct stat		st;
	t_content		*content;
	t_content		*lst;
	struct s_dir		*next;
	struct s_dir		*prev;
}				t_dir;

typedef struct			s_struct
{
	unsigned char		options;
	t_dir			*dir;
	t_dir			*curr;
	t_dir			*ldir;
}				t_struct;

int	read_dir(t_struct *d, char *name);
void	_error(t_struct *d, char *str, int error);
void	print_ls(t_struct *d);
void	_recursive(t_struct *d, t_content *content);
#endif
