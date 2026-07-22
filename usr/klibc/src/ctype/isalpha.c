int	ft_isalpha(char c)
{
	return (((unsigned int)c | 32) - 'a' < 26);
}
