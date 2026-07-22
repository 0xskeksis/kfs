int	ft_isspace(char c)
{
	return (c == ' ' || (unsigned)c - '\t' < 5);
}
