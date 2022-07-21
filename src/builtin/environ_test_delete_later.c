#include "environ.h"

int main(int ac, char **av, char **sys_envp)
{
	(void)ac;
	(void)av;

	char	**envp = new_environ(sys_envp);

	char **arglist = ft_calloc(3, sizeof(*arglist));
	arglist[2] = NULL;

	printf("\n------------------------------\n");
	printf("export\n");
	printf("------------------------------\n");
	arglist[0] = "export";
	exec_export(arglist, &envp);

	printf("\n------------------------------\n");
	printf("export TEST=\"test string...\"\n");
	printf("------------------------------\n");
	arglist[1] = "TEST=\"-------------this is test string---------------\"";
	exec_export(arglist, &envp);

	printf("\n------------------------------\n");
	printf("export 1NEW_TEST=\'test_string2\'\n");
	printf("------------------------------\n");
	arglist[1] = "1NEW_TEST=\'test_string2\'";
	exec_export(arglist, &envp);

	printf("\n------------------------------\n");
	printf("export\n");
	printf("------------------------------\n");
	arglist[1] = NULL;
	exec_export(arglist, &envp);


    printf("\n------------------------------\n");
	printf("env\n");
	printf("------------------------------\n");
	arglist[0] = "env";
	arglist[1] = NULL;
	exec_env(arglist, envp);

	printf("\n------------------------------\n");
	printf("unset TEST\n");
	printf("------------------------------\n");
	arglist[0] = "unset";
	arglist[1] = "TEST";
	exec_unset(arglist, &envp);

	printf("\n------------------------------\n");
	printf("env\n");
	printf("------------------------------\n");
	arglist[0] = "env";
	arglist[1] = NULL;
	exec_env(arglist, envp);


	free(arglist);
	delete_environ(&envp);

	return (0);
}
