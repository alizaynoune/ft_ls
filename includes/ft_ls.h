/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alzaynou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/17 16:45:13 by alzaynou          #+#    #+#             */
/*   Updated: 2020/10/30 05:35:02 by alzaynou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
# include <sys/xattr.h>
# include <sys/types.h>
# include <sys/acl.h>

/*
 ** return
 */

# define _SUCCESS	0
# define _FAILURE	1
# define _TROUBLE	2

/*
 ** flags
 */

# define _L			1
# define _R_		2
# define _R			4
# define _A			8
# define _T			16
# define _G			32
# define _U			64
# define _S			128
# define _F			256
# define _N			512

# define _MAX_OP	11

/*
 * index in tab lens
 * */
# define _LINK			0
# define _OWNER			1
# define _GROUP			2
# define _SIZE			3
# define _MAX_LEN_TABLE	4

/*
 ** colors
 */

# define C_BLK		""
# define C_CHR		"\e[1;33m"
# define C_DIR		"\e[1;34m"
# define C_FIFO		"\e[1;90m"
# define C_LNK		"\e[1;96m"
# define C_SOCK		"\e[1;35m"
# define C_EXE		"\e[1;32m"
# define C_DEF		"\e[1;0m"
# define C_ERROR	"\e[1;32m"

/*
* * permission
*/

# define P_X		1
# define P_W		2
# define P_R		4

/*
 * struct
 */

typedef struct			s_op
{
	char				c;
	char				*str;
	char				*desc;
	int					valu;
}						t_op;

typedef struct			s_waiting
{
	char				*name;
	char				*full_name;
	struct stat			*st;
	struct s_waiting	*next;
}						t_waiting;

typedef struct			s_files
{
	char				*name;
	char				*path;
	char				*link;
	struct stat			*st;
	struct passwd		*pwd;
	struct group		*grp;
	struct s_files		*next;
	struct s_files		*prev;
}						t_files;

typedef struct			s_dir
{
	ssize_t				total;
	char				*path;
	struct dirent		*dirent;
	t_files				*l_files;
	t_files				*h_files;
}						t_dir;

typedef struct			s_all
{
	int					ret;
	size_t				len[_MAX_LEN_TABLE];
	int					print_path;
	int					options;
	t_files				*arg_file;
	t_files				*l_arg_file;
	t_files				*files;
	t_files				*l_files;
	t_waiting			*head_waiting;
	t_waiting			*lst_waiting;
    t_dir               *dir;
}						t_all;

extern t_op				g_op[];

int						error_ls(t_all *d, char *err);
void					free_files(t_files **lst);
void					free_all(t_all *d);
void					push_files(t_all *d, t_files *f, t_files **lst, t_files **l_lst);
void					print_files(t_all *d, t_files *f);
void                    push_waiting(t_all *d, t_files *f);
void                    loop_dir(t_all *d);
void					parsing_files(t_all *d, char *f, t_files **lst, t_files **l_lst);
int						lstat_file(t_all *d, char *f, struct stat *st);
int						stat_file(t_all *d, char *f, struct stat *st);
t_files					*init_files(t_all *d, char *name, char *path);
void					free_dir(t_dir **dir);
void					loop_print_files(t_all *d, t_files *lst, t_files *l_lst , t_waiting *curr);
t_waiting				*init_waiting(t_all *d, t_files *f);
void					get_lens(t_all *d, t_files *f, char *name);
int                     init_id(t_all *d, t_files *new);
int						read_link(t_all *d, t_files *f);

#endif
