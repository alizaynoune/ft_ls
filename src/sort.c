# include "ft_ls.h"

int	_ls_alpha(char s1, char s2)
{
	s1 = s1 >= 'A' && s1 <= 'Z' ? s1 + 32 : s1;
	s2 = s2 >= 'A' && s2 <= 'Z' ? s2 + 32 : s2;
	return(s1 - s2);
}

int	_ls_alpha_helper(char s1, char s2)
{
	s1 += (s1 >= 'A' && s1 <= 'Z') ? 32 : -32;
	s2 += (s2 >= 'A' && s2 <= 'Z') ? 32 : -32;
	return (s1 - s2);
}

int	_ls_type(char s1, char s2)
{
	if (!s1 || !s2)
		return (s1 - s2);
	if (ft_isalnum(s1) && !ft_isalnum(s2))
		return (1);
	if (!ft_isalnum(s1) && ft_isalnum(s2))
		return (-1);
	if (!ft_isalnum(s1) && !ft_isalnum(s2))
		return (s1 - s2);
	if (ft_isalpha(s1) && ft_isdigit(s2))
		return (1);
	if (ft_isdigit(s1) && ft_isalpha(s2))
		return (-1);
	if (ft_isdigit(s1) && ft_isdigit(s2))
		return (s1 - s2);
	return (s1 - s2);
}


int	_ls_cmp(const char *s1, const char *s2)
{
	size_t	i;
	size_t	j;
	int	ret;

	i = 0;
	j = 0;
	ret = 0;
	while(!ret && (s1[i] || s2[j]))
	{
		while (s1[i] == '.')
			i++;
		while (s2[j] == '.')
			j++;
		ret = (ft_isalpha(s1[i]) && ft_isalpha(s2[j])) ?
			_ls_alpha(s1[i], s2[j]) : _ls_type(s1[i], s2[j]);
		s1[i] ? i++ : 0;
		s2[j] ? j++ : 0;
	}
	if (ret)
		return (ret);
	i = 0;
	j = 0;
	while(!ret && (s1[i] || s2[j]))
	{
		while (s1[i] == '.')
			i++;
		while (s2[j] == '.')
			j++;
		ret = (ft_isalpha(s1[i]) && ft_isalpha(s2[j])) ? _ls_alpha_helper(s1[i], s2[j]) : _ls_type(s1[i], s2[j]);
		s1[i] ? i++ : 0;
		s2[j] ? j++ : 0;
	}
	return (((ret) ? ret : ft_strhcmp(s1, s2)));
}
