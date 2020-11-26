#include "ft_ls.h"

void        read_dir(t_all *d, DIR *d_dir)
{
    struct dirent   *dirent;

    errno = 0;
    while ((dirent = readdir(d_dir)))
        parsing_read_file(d, d->dir->path, dirent->d_name);
    if (errno)
    {
        ft_dprintf(_ERR, "ls: cannot read directory '%s' %s\n",
                d->dir->path, strerror(errno));
        d->ret = _FAILURE;
        errno = 0;
    }
}

void		closs_dir(t_all *d)
{			
    if (closedir(d->fd_dir) == -1)
        ft_dprintf(_ERR, "ls: cannot close dir %s\n", strerror(errno));
    d->fd_dir = 0;
}

void        helper_loop_dir(t_all *d, t_waiting *tmp)
{ d->dir = init_dir(d, tmp->full_name);
    read_dir(d, d->fd_dir);
    closs_dir(d);
    ((d->options & _L) && d->dir->h_files) || d->options & _S ?
        ft_printf("total %d\n", d->dir->total) : 0;
    loop_print_files(d, d->dir->h_files, d->dir->l_files, tmp);
    free_dir(&d->dir);

}

void        loop_dir(t_all *d)
{
    t_waiting       *tmp;

    tmp = d->head_waiting;
    (tmp && tmp->next) ? d->print_path = _SUCCESS : 0;
    while (tmp)
    {
        ft_bzero((void *)d->len, sizeof(d->len));
        (d->print_path == _SUCCESS) ? ft_printf("%s:\n", tmp->full_name) : 0;
        if ((d->fd_dir = opendir(tmp->full_name)))
            helper_loop_dir(d, tmp);
        else if (errno == ENOMEM)
            error_ls(d, strerror(errno));
        else
        {
            ft_dprintf(_ERR, "ls: cannot open directory '%s': %s\n",
                    tmp->name, strerror(errno));
            d->ret = _FAILURE;
        }
        tmp = tmp->next;
        free_printed(d);
        (tmp) ? ft_printf("\n") : 0;
    }
}
