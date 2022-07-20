# include <stdlib.h>
# include <unistd.h> //STDOUT, IN
# include "../libft/include/libft.h"

# define TRUE 1
# define FALSE 0

// # define FILENO_STDOUT 0

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

	if (!ft_strncmp(str, "-n", 2))
	{
		i = 2;
		while (str[i] == 'n')
			i++;
		if (str[i] == '\0')
				return (TRUE);
	}
	return (FALSE);
}

static void	write_arguments(char **arglist, size_t i)
{
	while (arglist[i])
	{
		ft_putstr_fd(arglist[i], STDOUT_FILENO);
		if (arglist[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
}

void	exec_echo(char **arglist, char **our_envp)
{
	int		n_option;

	(void)our_envp;
	n_option = FALSE;
	if (arglist[1] != NULL)
	{
		if (is_n_option(arglist[1]) == TRUE)
			n_option = TRUE;
		write_arguments(arglist, 1 + n_option);
	}
	if (n_option == FALSE)
		ft_putchar_fd('\n', STDOUT_FILENO);
}

//gcc -g echo.c ../libft/src/ft_putstr_fd.c ../libft/src/ft_putchar_fd.c ../libft/src/ft_strncmp.c ../libft/src/ft_strlen.c
/** int main() */
/** { */
/**     char **arglist = calloc(8, sizeof(char *)); */
/**     arglist[0] = "echo"; */
/**     arglist[1] = "-nnn"; */
/**     arglist[2] = "a  dd"; */
/**     arglist[3] = "b"; */
/**  */
/**     exec_echo(arglist); */
/** } */
