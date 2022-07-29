/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minkyeki <minkyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 16:22:56 by minkyeki          #+#    #+#             */
/*   Updated: 2022/07/29 16:25:17 by minkyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_H
# define HEREDOC_H

# include "../lexer/token.h"

void	expand_quote(t_string *str, t_iterator *iter, char quote_type);
bool	is_limiter(const char *line, const char *limiter);
char	*readline_prompt_heredoc(void);
void	set_heredoc(t_list *tokens);

#endif /* HEREDOC_H */
