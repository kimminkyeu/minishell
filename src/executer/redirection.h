#ifndef REDIRECTION_H
# define REDIRECTION_H

#include "../main/minishell.h"
#include "../lexer/token.h"

char	*expand_line_each(char *line, t_shell_config *config);
void	expand_file(t_token *tok, t_shell_config *config);

#endif/* REDIRECTION_H */
