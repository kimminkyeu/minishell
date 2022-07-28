#ifndef HEREDOC_H
# define HEREDOC_H

#include "../lexer/token.h"

void	expand_quote(t_string *str, t_iterator *iter, char quote_type);
bool	is_limiter(const char *line, const char *limiter);
char	*readline_prompt_heredoc(void);

#endif /* HEREDOC_H */
