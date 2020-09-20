# include "ft_dprintf.h"
# include <stdio.h>
int	main(int ac, char **av)
{
		ft_dprintf(1, "%d\n", ft_strhcmp(av[1], av[2]));
}
