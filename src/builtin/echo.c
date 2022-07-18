#include "../parser/parse_tree.h"

/*
** example: [echo] [-nnn] [a] [b] [-n] [c]
** 1) check 2th node
** 1-1) if NULL -> do nothing
** 1-2) if -n && only n detected -> -n option on
** 2) print from 3th to last
*/

int	is_n_option(char *text)
{
	else if (ft_strncmp(cur->content->str->text, "-n", 2))
	while ()
}

void	builtin_echo(t_list *token_list)
{
	t_list	*cur;
	int		n_option;

	cur = token_list;
	cur = cur->next;
	if (cur == NULL)
		return ;
	n_option = FALSE;
	if (is_n_option(cur->next->content->str->text) == TRUE)
		n_option = TRUE;

}

void	set_redirection(t_list *redir_list)
{
	t_list	*cur;

	close(/*원래 in*/);
	close(/*원래 out*/);
	cur = redir_list;
	while (cur)
	{
		//
	}
}
