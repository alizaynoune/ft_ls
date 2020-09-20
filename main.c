# include <stdio.h>
# include <limits.h>

int		main()
{
	int i = 0;
	while (i <= CHAR_MAX)
		dprintf(1, " a%ca ", i++);
}
