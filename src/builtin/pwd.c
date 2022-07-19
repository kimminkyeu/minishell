# include <unistd.h> //getcwd()
# include <sys/param.h> // define MAXPATHLEN 1024 (man getwd)
# include "../libft/include.libft.h"

/*
** NOTE1: It is independent of environment variable PWD.
** NOTE2: All arguments are ignored.
*/

void	exec_pwd(char **arglist)
{
	char	*buf;

	buf = ft_calloc(1, MAXPATHLEN);
	getcwd(buf, sizeof(*buf));
	ft_putstr_fd(buf, FILENO_STDOUT);
	free(buf);
}