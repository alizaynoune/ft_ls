#include "ft_ls.h"

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

void        get_len_block(t_all *d, t_files *f)
{
	size_t      len;

	len = ft_intlen(f->st->st_blocks);
	(len > d->len[_BLOCK]) ? d->len[_BLOCK] = len : 0;
}

void        get_len_inode(t_all *d, t_files *f)
{
	size_t      len;

	len = ft_uintlen(f->st->st_ino);
	(len > d->len[_INODE]) ? d->len[_INODE] = len : 0;
}

void        len_major_minor(t_all *d, t_files *f)
{
	size_t      len;

	len = ft_intlen(major(f->st->st_rdev));
	(len > d->len[_MAJ_MIN]) ? d->len[_MAJ_MIN] = len : 0;
	len = ft_intlen(minor(f->st->st_rdev));
	(len > d->len[_MAJ_MIN]) ? d->len[_MAJ_MIN] = len : 0;
	(d->len[_MAJ_MIN] < 3) ? d->len[_MAJ_MIN] = 3 : 0;
}

void        get_lens(t_all *d, t_files *f)
{
	size_t      len;

	len = ft_intlen(f->st->st_nlink);
	((d->len[_LINK] < len)) ? d->len[_LINK] = len : 0;
	len = ft_intlen(f->st->st_size);
	((d->len[_SIZE] < len)) ? d->len[_SIZE] = len : 0;
	len = (!(d->options & _N) && f->pwd && f->pwd->pw_name) ?
        ft_strlen(f->pwd->pw_name) : ft_uintlen(f->st->st_uid);
	(len > d->len[_OWNER]) ? d->len[_OWNER] = len : 0;
	len = (!(d->options & _N) && f->grp && f->grp->gr_name) ?
        ft_strlen(f->grp->gr_name) :
		ft_uintlen(f->st->st_gid);
	(len > d->len[_GROUP]) ? d->len[_GROUP] = len : 0;
	((d->options & _S)) ? get_len_block(d, f) : 0;
	((S_ISCHR(f->st->st_mode)) || (S_ISBLK(f->st->st_mode))) ?
        len_major_minor(d, f) : 0;
	((d->options & _I)) ? get_len_inode(d, f) : 0;
}
