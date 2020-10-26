#include "ft_ls.h"

t_dir       *init_dir(t_all *d, char *name)
{
    t_dir   *new;

    if (!(new = (t_dir *)ft_memalloc(sizeof(t_dir))))
        error_ls(d, strerror(errno));
    if (!(new->path = ft_strjoin(name, "/")))
    {
        free(new);
        error_ls(d, strerror(errno));
    }
    return (new);
}

void        read_dir(t_all *d, DIR *d_dir)
{
    struct dirent   *dirent;

    errno = 0;
    while ((dirent = readdir(d_dir)))
        ft_printf("%s\n", dirent->d_name);
    if (errno)
    {
        ft_dprintf(2, "ls: cannot read directory%s\n", strerror(errno));
        d->ret = _FAILURE;
        errno = 0;
    }
}

void        loop_dir(t_all *d)
{
    t_waiting       *tmp;
    DIR             *d_dir;

    tmp = d->head_waiting;
    while (tmp)
    {
        if ((d_dir = opendir(tmp->name)))
        {
            d->dir = init_dir(d, tmp->name);
            read_dir(d, d_dir);
            if (closedir(d_dir) == -1)
                ft_dprintf(2, "ls: cannot close dir%s\n", strerror(errno));
        }
        else
        {
            ft_dprintf(2, "ls: cannot open directory '%s/': %s\n",
                    tmp->name, strerror(errno));
            d->ret = _FAILURE;
        }
        tmp = tmp->next;
    }
}
