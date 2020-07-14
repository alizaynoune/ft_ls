#include "libft.h"

char		*ft_nstrjoin(size_t n, ...)
{
	char		*fresh;
	char		*ptr;
	va_list		ap;

	va_start(ap, n);
	if (!n || !(fresh = ft_strdup(va_arg(ap, char *))))
		return (NULL);
	while (--n)
	{
		ptr = fresh;
		if (!(fresh = ft_strjoin(fresh, va_arg(ap, char *))))
		{
			free(fresh);
			return (NULL);
		}
		free(ptr);
	}
	va_end(ap);
	return (fresh);
}
