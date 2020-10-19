#include "ft_dprintf.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int	main(int ac, char **av)
{
	int	ft = 0;
	int	sys = 0;
	int	fd;
	char	*str;
	long long int value;
	long long int prec;
	fd = 1;
	if (ac >= 4 && av[1][0] == 'd')
	{
		prec = atoll(av[2]);
		str = av[3];
		value = atoll(av[4]);
		dprintf(fd, "[ret %d]\n",ft_dprintf(fd, str, prec , value));
		dprintf(fd, "[ret %d]\n",dprintf(fd, str, value));
	}
	if (ac >= 4 && av[1][0] == 's')
	{
		prec = atoll(av[2]);
		str = av[3];
		dprintf(fd, "[ret %d]\n",ft_dprintf(fd, str, prec, av[3]));
		dprintf(fd, "[ret %d]\n",dprintf(fd, str, prec, av[3]));
	}
}
