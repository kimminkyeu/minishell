# include <unistd.h>//getcwd(), STDOUT
# include <stdio.h>//perror()
# include <sys/param.h>//define MAXPATHLEN 1024 (man getwd)
# include "../libft/include/libft.h"

/*
** NOTE1: It is independent of environment variable PWD.
** NOTE2: All arguments are ignored.
*/

void	exec_pwd(char **arglist/* 인자는 사용되지 않는다. */)
{
	char	*buf;

	buf = ft_calloc(1, MAXPATHLEN);
	if (getcwd(buf, MAXPATHLEN) == NULL)
		perror("lesh: pwd");
	ft_putstr_fd(buf, STDOUT_FILENO);
	free(buf);
}

//gcc -g pwd.c ../libft/src/ft_calloc.c ../libft/src/ft_putstr_fd.c ../libft/src/ft_bzero.c ../libft/src/ft_strlen.c
int main()
{
	char **arglist = calloc(8, sizeof(char *));
	arglist[0] = "pwd";
	arglist[1] = "a";
	arglist[2] = "c";
	arglist[3] = "b";

	exec_pwd(arglist);
}