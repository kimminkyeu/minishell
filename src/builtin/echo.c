#include "../lexer/scanner.h"
#include "../parser/parse_tree.h"

# define TRUE 1;
# define FALSE 0;

/*
** example: [echo] [-nnn] [a] [b] [-n] [c]
** 1) check 2th node
** 1-1) if NULL -> do nothing
** 1-2) if -n && only n detected -> -n option on
** 2) print from 3th to last
*/

static int	is_n_option(char *str)
{
	size_t	i;

	i = 0;
	if (ft_strncmp(str, "-n", 2))
	{
		i = i + 2;
		while (str[i] == 'n')
			i++;
		if (str[i] == '\0')
				return (TRUE);
	}
	else
		return (FALSE);
}

static void	write_arguments(char **arglist, size_t i)
{
	while (arglist[i])
	{
		ft_putstr_fd(cur->content->string->str, FILENO_STDOUT);
		if (arglist[i + 1])
			ft_putchar_fd(' ', FILENO_STDOUT);
		i++;
	}
}

void	exec_echo(char **arglist)
{
	int		n_option;

	n_option = FALSE;
	if (arglist[1] != NULL)
	{
		if (is_n_option(arglist[1]) == TRUE)
			n_option = TRUE;
		write_arguments(arglist, 1 + n_option);
	}
	if (n_option == FALSE)
		ft_putchar_fd('\n', FILENO_STDOUT);
}
