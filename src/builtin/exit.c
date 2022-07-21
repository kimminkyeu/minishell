# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <limits.h>
# include "../libft/include/libft.h"

# define EXIT_FAILURE 1
// # define EXIT_BADUSAGE 2

/* exit [n]: exit with status of n.
** 1. no [n]: exit with exit status of last command.
** 2-1. [n] more than 1: command exit status 1. !!exit fail!!
** 2-2. [n] is not number: exit(255).
*/

/* how exit() work with argument:
** 1) atoll(alpha to longlong)
** 1-1) if over/underflow occurs, it's not a number.
** 2) cast into unsigned char
*/

static bool	get_status(char *str, unsigned char *status)
{
	int					neg;
	unsigned long long	num;
	size_t				i;

	i = 0;
	neg = 1;
	if (str[i] == '-')
		neg = -1;
	if (str[i] == '+' || str[i] == '-')
		i++;
	num = 0;
	while ('0' <= str[i] && str[i] <= '9')
	{
		num = num * 10 + (str[i] - '0');
		if (neg == -1 && num > LLONG_MIN || neg == 1 && num > LLONG_MAX)
			return (false);
		i++;
	}
	if (str[i] == '\0')
	{
		*status = neg * num;
		return (true);
	}
	else
		return (false);
}

int	exec_exit(char **arglist, char **our_envp/*, pipeline command?, 마지막 exit status */)
{
	unsigned char	status;

	(void)our_envp;

	// if (파이프라인 커맨드가 아닐 때만)
		ft_putstr_fd("exit\n", STDOUT_FILENO);
	if (arglist[1] == NULL)
		status = 0/* TODO: 마지막으로 실행된 명령어의 exit status로 변경*/;
	else if (get_status(arglist[1], &status) == false)
	{
		status = 255/*DISCUSS: 255는 의미 없는 에러메시지다. jkong님은 EXIT_BADUSAGE(2)를 출력함.*/;
		ft_putstr_fd("lesh: exit: ", STDERR_FILENO);
		ft_putstr_fd(arglist[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	}
	else if (arglist[2] != NULL)
	{
		ft_putstr_fd("lesh: exit: too many arguments\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	exit(status);/*부모에게 전달하고 종료.*/
}

//gcc exit.c ../libft/src/ft_putstr_fd.c ../libft/src/ft_strlen.c

int main(int argc, char **argv, char **envp)
{
	exec_exit(argv + 1, envp);
}